#include "CollisionTriangle2D.h"

float Triangle2D::len(int index) const
{
	// index　= 0は辺p1p2など点piの対辺の長さ
	return vec(index).Length();
}

// 辺ベクトル(非正規化)を取得
Vector2 Triangle2D::vec(int index) const
{
	int idx[] = { 1,2,0,1 };
	return vertices[idx[index + 1]] - vertices[idx[index]];
}
