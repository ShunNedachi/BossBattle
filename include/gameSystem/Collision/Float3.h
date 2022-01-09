#pragma once

class Float3
{
public:

	Float3():x(0),y(0),z(0){}
	Float3(float x, float y, float z) :x(x), y(y), z(z){}
	~Float3(){}

    // 四則演算
    Float3 operator +(const Float3& temp) const;
    Float3 operator -(const Float3& temp) const;
    Float3 operator *(const Float3& temp) const;
    Float3 operator /(const Float3& temp) const;
    Float3 operator +(float temp)const;
    Float3 operator -(float temp)const;
    Float3 operator *(float temp) const;
    Float3 operator /(float temp) const;

    // 代入演算子オーバーロード
    Float3& operator =(const Float3& temp);

    float Dot(const Float3& temp) const;
    Float3 Cross(const Float3& temp) const;
    float Length() const;
    void Normalize();

public:

	float x, y, z;

};

