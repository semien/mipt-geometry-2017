#pragma once
#include<thread>
#include<deque>
#include<condition_variable>
#include<mutex>
#include<functional>
#include<future>
#include<exception>
#include<atomic>
#include<iostream>

template <class Value, class Container = std::deque<Value>>
class thread_safe_queue {
public:
	explicit thread_safe_queue(size_t capacity) :cap(capacity)
	{
		enable.store(true);
	};
	void enqueue(Value v) {
		std::unique_lock<std::mutex> lock(mtx);
		if (enable.load() == false) {
			throw std::bad_function_call();
		}
		if (container.size() >= cap) {
			full.wait(lock, [&]() {return container.size() < cap; });
		}
		if (enable.load() == false) {
			throw std::bad_function_call();
		}
		container.push_back(std::move(v));
		empty.notify_one();
	};
	void pop(Value& v) {
		std::unique_lock<std::mutex> lock(mtx);
		while (container.empty()) {
			if (enable.load() == false) {
				throw std::bad_function_call();
			}
			empty.wait(lock);
		}
		v = std::move(container.front());
		container.pop_front();
		full.notify_one();
	};
	void shutdown() {
		enable.store(false);
		full.notify_all();
		empty.notify_all();
	};
private:
	std::mutex mtx;
	std::condition_variable empty;
	std::condition_variable full;
	std::atomic<bool> enable;
	size_t cap;
	Container container;
};

const size_t INF = std::numeric_limits<size_t>::max();

template <class Value>
class thread_pool {
public:
	explicit thread_pool(size_t num_workers) :  tasks(INF)
	{
		for (size_t i = 0; i < num_workers; ++i) {
			threads.push_back(std::thread([this]() {this->threadFunc(); }));
		}
	};
	std::future<Value> submit(std::function<Value()> func) {
		std::packaged_task<Value()> ptask(func);
		std::future<Value> future = ptask.get_future();
		tasks.enqueue(std::move(ptask));
		return std::move(future);
	};
	void shutdown() {
		tasks.shutdown();
		for (size_t i = 0; i < threads.size(); ++i) {
			threads[i].join();
		}
	};
	void threadFunc() {
		try {
			while (true) {
				std::packaged_task<Value()> task;
				tasks.pop(task);
				task();
			}
		}
		catch (std::bad_function_call e) {
			return;
		}
	};
private:
	std::vector<std::thread> threads;
	thread_safe_queue<std::packaged_task<Value()>> tasks;
};

