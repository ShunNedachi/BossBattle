#pragma once
#include"CollisionPoint.h"

class Circle2D
{
public:
	// ƒƒ“ƒoŠÖ”
	Circle2D():point(0,0),radius(0){}
	Circle2D(const Point2D& p, float r):point(p),radius(r){}
	~Circle2D(){}

public:

	Point2D point;
	float radius; 

};

