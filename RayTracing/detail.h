#pragma once
#include<algorithm>
#include<vector>
#include"basicStructures.h"
#include"primitives.h"
#include"functions.h"

Point m(0,0,0);

Colour min(Colour col) {
	return Colour{ std::min(col.red,(double)255),std::min(col.green,(double)255) ,std::min(col.blue,(double)255) };
}

Colour average(Colour c1, Colour c2, Colour c3, Colour c4) {
	return Colour{ (c1.red + c2.red + c3.red + c4.red) / 4, (c1.green + c2.green + c3.green + c4.green) / 4, (c1.blue + c2.blue + c3.blue + c4.blue) / 4, };
}

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
	Scene(std::vector<Object*>& objs, std::vector<Light*> lighters, Point spectator): 
		spectator_(spectator)
		, sceneObjects_(objs)
		, lighters_(lighters)
	{}	
	
	void setPixels(Screen& scr, std::vector<Colour>& colors) {
		for (int i = 0; i < scr.vert; ++i) {
			for (int j = 0; j < scr.horiz; ++j) {
				Point x = scr.angle + scr.b*i + scr.a*j;
				Colour c1 = findColor(Line{ spectator_, x - spectator_ });
				Point x2 = x + scr.b * 0.5;
				Colour c2 = findColor(Line{ spectator_, x2 - spectator_ });
				Point x3 = x + scr.a * 0.5;
				Colour c3 = findColor(Line{ spectator_, x3 - spectator_ });
				Point x4 = x3 + scr.b * 0.5;
				Colour c4 = findColor(Line{ spectator_, x4 - spectator_ });
				colors.push_back(average(c1, c2, c3, c4));
			}
		}
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
			if (obj->rayIntersect(ray, z, dist, col) && dist < distance) {
				intersectObj = obj;
				distance = dist;
				intersPoint = z;
				color = col;
			}
		}

		if (intersectObj == nullptr) {
			return Colour{ -1,-1,-1 };
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
				if (object->rayIntersect(newRay, u, dist2, col2) && dist2 < maxDistance) {
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
			if (obj->rayIntersect(ray, z, dist, col) && dist < distance) {
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
				if (object->rayIntersect(newRay, u, dist2, col2) && dist2 < maxDistance) {
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
		if (reflColor.red != -1) {
			color = color*(1 - intersectObj->getReflection()) + reflColor*intersectObj->getReflection();
		}
		return color;
	}

	Point spectator_;
	std::vector<Object*> sceneObjects_;
	std::vector<Light*> lighters_;
};