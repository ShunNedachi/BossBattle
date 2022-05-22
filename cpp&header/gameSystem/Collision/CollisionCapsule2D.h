#pragma once
#include"CollisionSegment2D.h"


class Capsule2D
{
public:
	// ƒƒ“ƒoŠÖ”
	Capsule2D():segment(Point2D(0,0),Vector2(0,0)),radius(0) {}
	Capsule2D(const Segment2D& tempSeg, float r):segment(tempSeg),radius(r){}
	Capsule2D(const Point2D& p1, const Point2D& p2, float r):segment(p1,p2),radius(r){}
	~Capsule2D(){}


public:
	// ƒƒ“ƒo•Ï”
	Segment2D segment;
	float radius; // ”¼Œa
};

