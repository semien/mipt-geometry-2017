#pragma once
#include<algorithm>
#include<vector>
#include"basicStructures.h"
#include"primitives.h"
#include"functions.h"
#include"thread_pool.h"

extern const int SCREEN_WIDTH;

Point m(0,0,0);

Colour min(Colour col) {
	return Colour{ std::min(col.red,(double)255),std::min(col.green,(double)255) ,std::min(col.blue,(double)255) };
}

Colour average(Colour c1, Colour c2, Colour c3, Colour c4) {
	return Colour{ (c1.red + c2.red + c3.red + c4.red) / 4, (c1.green + c2.green + c3.green + c4.green) / 4, (c1.blue + c2.blue + c3.blue + c4.blue) / 4, };
}

class Screen {
public:
	Screen() {}
	Screen(Point& p1, Point& p2, Point& p3, Point& p4, int hor, int vert) :
		horiz(hor)
		, vert(vert)
	{
		a = (p1 - p2)/ vert;
		b = (p3 - p2)/ hor;
		angle = p2;
	}
	Screen(Screen &other) :
		a(other.a)
		, b(other.b)
		, angle(other.angle)
		, horiz(other.horiz)
		, vert(other.vert)
	{}
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
	Scene(Scene& other) :
		spectator_(other.spectator_)
		, sceneObjects_(other.sceneObjects_)
		, lighters_(other.lighters_)
		, scr(other.scr)
	{}
		
	Scene(std::vector<Object*>& objs, std::vector<Light*> lighters, Point spectator, Screen& scr): 
		spectator_(spectator)
		, sceneObjects_(objs)
		, lighters_(lighters)
		, scr(scr)
	{}	

	void setPixel(int i, int j, std::vector<Colour>& colors) {
		Point screenPoint1 = scr.angle + scr.b*i + scr.a*j;
		Line ray1{ spectator_, screenPoint1 - spectator_ };
		Colour col1 = findColor(ray1);

		/*Point screenPoint2 = scr.angle + scr.b*i + scr.b*0.5 + scr.a*j;
		Line ray2{ spectator_, screenPoint2 - spectator_ };
		Colour col2 = findColor(ray2);

		Point screenPoint3 = scr.angle + scr.b*i + scr.a*j + scr.a*0.5;
		Line ray3{ spectator_, screenPoint3 - spectator_ };
		Colour col3 = findColor(ray3);

		Point screenPoint4 = scr.angle + scr.b*i + scr.b*0.5 + scr.a*j + scr.a*0.5;
		Line ray4{ spectator_, screenPoint4 - spectator_ };
		Colour col4 = findColor(ray4);*/

		colors[i*SCREEN_WIDTH + j] = col1;//average(col1, col2, col3, col4);
	}
	
	void newSpec(Point newSpec) {
		spectator_ = newSpec;
	}

	void newScreen(Screen screen) {
		scr = screen;
	}

	~Scene(){}
private:
	Colour findSecondColor(Line& ray) {
		Point z;
		Point intersPoint;
		Object* intersectObj = nullptr;
		Colour color;
		ld distance = 1000000;
		for (auto obj : sceneObjects_) {
			ld dist;
			Colour col;
			if (obj->rayIntersect(ray, z, dist, col, 1)  && dist < distance) {
				intersectObj = obj;
				distance = dist;
				intersPoint = z;
				color = col;
			}
		}

		if (intersectObj == nullptr) {
			return Colour{ 0,0,0 };
		}

		Point u;
		float coeff = 0;
		float intens = -1;
		for (auto lighter : lighters_) {
			intens = -1;
			Point directionLight = lighter->a - intersPoint;
			ld maxDistance = directionLight.len();
			ld dist2;
			Colour col2;
			Line newRay{ intersPoint,directionLight };
			for (auto object : sceneObjects_)
				if (object->rayIntersect(newRay, u, dist2, col2, 0) && dist2 < maxDistance) {
					intens = 0;
					break;
				}
			if (intens == 0) {
				continue;
			}
			else {
				intens = lighter->intensity;
			}
			coeff += intens*abs(intersectObj->getNormal(intersPoint)*directionLight / (maxDistance*maxDistance*directionLight.len()));
		}
		color = min(color*coeff);
		return color;
	}

	Colour findColor(Line& ray) {
		Point z;
		Point intersPoint;
		Object* intersectObj = nullptr;
		Colour color;
		ld distance = 1000000;
		for (auto obj : sceneObjects_) {
			ld dist;
			Colour col;
			if (obj->rayIntersect(ray, z, dist, col , 0) && dist < distance) {
				intersectObj = obj;
				distance = dist;
				intersPoint = z;
				color = col;
			}
		}
		
		if (intersectObj == nullptr) {
			return Colour{ 0,0,0 };
		}

		Point u;
		float coeff = 0;
		float intens=-1;
		for (auto lighter : lighters_) {
			intens = -1;
			Point directionLight = lighter->a - intersPoint;
			ld maxDistance = directionLight.len();
			ld dist2;
			Colour col2;
			Line newRay{ intersPoint,directionLight };
			for (auto object : sceneObjects_)
				if (object->rayIntersect(newRay, u, dist2, col2, 0) && dist2 < maxDistance) {
					intens = 0;
					break;
				}
			if (intens == 0) {
				continue;
			}
			else {
				intens = lighter->intensity;
			}
			coeff += intens*abs(intersectObj->getNormal(intersPoint)*directionLight / (maxDistance*maxDistance*directionLight.len()));
		}
		color = min(color*coeff);
		Colour reflColor = findSecondColor(intersectObj->reflRay(ray, intersPoint));
		//if (reflColor.red != -1) {
			color = color*(1 - intersectObj->getReflection()) + reflColor*intersectObj->getReflection();
		//}
		return color;
	}
	Screen scr;
	Point spectator_;
	std::vector<Object*> sceneObjects_;
	std::vector<Light*> lighters_;
};