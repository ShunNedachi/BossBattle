#pragma once
#include"CollisionPoint.h"

class Sphere
{
public:
	// メンバ関数
	Sphere() :point(Point3D(0, 0, 0)), radius(0){}
	Sphere(const Point3D& point,float radius):point(point),radius(radius){}
	~Sphere(){}

public:
	// メンバ変数
	Point3D point;
	float radius;
};

