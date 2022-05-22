#pragma once
#include"CollisionLine2D.h"

class Segment2D :public Line2D
{
public:

	Segment2D():Line2D(Point2D(0,0),Vector2(0,0)) {}

	Segment2D(const Point2D& p, const Vector2& vector):Line2D(p, vector) {}
	// p1 �n�_�@p2 �I�_
	Segment2D(const Point2D & p1, const Point2D & p2):Line2D(p1, p2 - p1) {}

};