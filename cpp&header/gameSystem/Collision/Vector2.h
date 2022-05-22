#pragma once
#include"Float2.h"

class Vector2 : public Float2
{
public:
	Vector2() {}
	Vector2(float x, float y) :Float2(x, y) {}
	Vector2(const Float2& temp) : Float2(temp){}
	~Vector2() {}

	Vector2& operator = (const Float2& temp);
};

