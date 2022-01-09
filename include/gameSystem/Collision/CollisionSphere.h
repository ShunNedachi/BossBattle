#pragma once
#include"CollisionPoint.h"

class Sphere
{
public:
	// ƒƒ“ƒoŠÖ”
	Sphere() :point(Point3D(0, 0, 0)), radius(0){}
	Sphere(const Point3D& point,float radius):point(point),radius(radius){}
	~Sphere(){}

public:
	// ƒƒ“ƒo•Ï”
	Point3D point;
	float radius;
};

