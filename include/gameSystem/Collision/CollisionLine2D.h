#pragma once
#include"CollisionPoint.h"
#include"Vector2.h"

class Line2D
{
public:
	// �����o�֐�

	// �R���X�g���N�^�@�f�X�g���N�^
	Line2D():point(0,0),dirV(0,0) {}
	Line2D(const Point2D& point, const Vector2& vector):point(point),dirV(vector){}
	~Line2D(){}

	// �_��̍��W���擾
	Point2D GetPoint()const { return point; }

public:
	// �����o�ϐ�

	Point2D point;
	Vector2 dirV; // �����x�N�g��
};

