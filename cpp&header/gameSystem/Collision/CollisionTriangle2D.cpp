#include "CollisionTriangle2D.h"

float Triangle2D::len(int index) const
{
	// index�@= 0�͕�p1p2�ȂǓ_pi�̑Εӂ̒���
	return vec(index).Length();
}

// �Ӄx�N�g��(�񐳋K��)���擾
Vector2 Triangle2D::vec(int index) const
{
	int idx[] = { 1,2,0,1 };
	return vertices[idx[index + 1]] - vertices[idx[index]];
}
