#pragma once
#include<vector>
#include"basicStructures.h"
#include"primitives.h"


class Screen {
public:
	Screen(Point& p1, Point& p2, Point& p3, Point& p4, int hor, int vert) :
		horiz(hor)
		, vert(vert)
	{
		a = (p1 - p2)/ vert;
		b = (p3 - p2)/ hor;
		angle = p2;
	}
	Point a; //vert
	Point b; //hor
	Point angle; //left up
	int horiz;
	int vert;
	~Screen() {}
};

class Scene {
public:
	Scene() {}
	Scene(std::vector<Object*>& objs, Point spectator): spectator(spectator), sceneObjects(objs) {}	
	void setPixels(Screen& scr, std::vector<Colour>& colors) {
		for (int i = 0; i < scr.vert; ++i) {
			for (int j = 0; j < scr.horiz; ++j) {
				Point x = scr.angle + scr.b*i + scr.a*j;
				Line ray(spectator, x);
				Point z;
				Colour color{ 0, 0, 0 };
				ld distance = 1000000;
				for (auto obj : sceneObjects) {
					ld dist;
					Colour col;
					if (obj->rayIntersect(ray, z, dist, col) && dist<distance) {
						color = col;
						distance = dist;
					}
				}
				colors.push_back(color);
			}
		}
	}

	Point spectator;
	std::vector<Object*> sceneObjects;

	~Scene(){}
};