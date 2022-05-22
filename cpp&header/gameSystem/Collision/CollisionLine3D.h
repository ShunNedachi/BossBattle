#pragma once
#include"CollisionPoint.h"
#include"Vector3.h"

class Line3D
{
public:
	// メンバ関数
	Line3D():point(0,0,0),dirVector(0,0,0){}
	Line3D(const Point3D& point, const Vector3& vector) :point(point), dirVector(vector){}


public:
	// メンバ変数

	// 始点
	Point3D point;
	// 方向ベクトル
	Vector3 dirVector;
};

