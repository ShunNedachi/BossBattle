#pragma once
#include"CollisionPoint.h"
#include"Vector2.h"

class Triangle2D
{
	// メンバ関数
public:
	Triangle2D() {}
	~Triangle2D(){}

	float len(int index)const;


private:
	// 辺ベクトルの取得
	Vector2 vec(int index)const;

	// メンバ変数
public:
	Point2D vertices[3]; // 頂点座標
};

