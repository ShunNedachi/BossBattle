#pragma once
#include"Vector3.h"

class Plane
{

public:
	// 今後追加する可能性を考慮

	// 法線ベクトル
	Vector3 normal = { 0,1,0 };
	// 原点からの距離
	float distance = 0.0f;

};