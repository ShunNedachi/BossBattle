#pragma once
#include"CollisionSegment3D.h"

class Capsule3D
{
public:
	// メンバ関数
	
	Capsule3D():segment(Segment3D()),radius(0){}
	Capsule3D(const Segment3D& segment, float r) :segment(segment), radius(r){}
	Capsule3D(const Point3D& p1,const Point3D& p2,float r):segment(Segment3D(p1,p2)),radius(r){}
	~Capsule3D(){}

public:
	// メンバ変数
	
	Segment3D segment;
	float radius;
};

