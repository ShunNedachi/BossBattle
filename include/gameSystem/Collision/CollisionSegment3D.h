#pragma once
#include"CollisionLine3D.h"

class Segment3D:public Line3D
{
public:
	// ƒƒ“ƒoŠÖ”
	

	Segment3D() :Line3D(Point3D(0, 0, 0), Vector3(0, 0, 0)){}
	Segment3D(const Point3D& point, const Vector3& vector) :Line3D(point, vector){}
	Segment3D(const Point3D& p1, const Point3D& p2) :Line3D(p1, p2 - p1){}
};

