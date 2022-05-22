#pragma once
#include"Float3.h"

class Vector3:public Float3
{
public:
	// メンバ関数

	Vector3():Float3(0,0,0){}
	Vector3(float x, float y, float z):Float3(x,y,z){}
	Vector3(const Float3& temp):Float3(temp){}
	~Vector3(){}

	// 代入演算子オーバーロード
	Vector3& operator =(const Float3& temp);
};

