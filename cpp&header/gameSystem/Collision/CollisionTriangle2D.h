#pragma once
#include"CollisionPoint.h"
#include"Vector2.h"

class Triangle2D
{
	// �����o�֐�
public:
	Triangle2D() {}
	~Triangle2D(){}

	float len(int index)const;


private:
	// �Ӄx�N�g���̎擾
	Vector2 vec(int index)const;

	// �����o�ϐ�
public:
	Point2D vertices[3]; // ���_���W
};

