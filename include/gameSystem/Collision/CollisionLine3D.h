#pragma once
#include"CollisionPoint.h"
#include"Vector3.h"

class Line3D
{
public:
	// �����o�֐�
	Line3D():point(0,0,0),dirVector(0,0,0){}
	Line3D(const Point3D& point, const Vector3& vector) :point(point), dirVector(vector){}


public:
	// �����o�ϐ�

	// �n�_
	Point3D point;
	// �����x�N�g��
	Vector3 dirVector;
};

