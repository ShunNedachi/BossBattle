#pragma once
#include"CollisionPoint.h"
#include"Vector2.h"

class Line2D
{
public:
	// メンバ関数

	// コンストラクタ　デストラクタ
	Line2D():point(0,0),dirV(0,0) {}
	Line2D(const Point2D& point, const Vector2& vector):point(point),dirV(vector){}
	~Line2D(){}

	// 点上の座標を取得
	Point2D GetPoint()const { return point; }

public:
	// メンバ変数

	Point2D point;
	Vector2 dirV; // 方向ベクトル
};

