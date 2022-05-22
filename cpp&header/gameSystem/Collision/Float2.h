#pragma once

// float2つを内包するクラス
class Float2
{
public: // メンバ関数

	Float2():x(0),y(0) {}
	Float2(float x, float y):x(x),y(y) {}
	~Float2() {}
	
	// 四則演算用
	Float2 operator +(const Float2& temp);
	Float2 operator -(const Float2& temp)const;
	Float2 operator *(const Float2& temp)const;
	Float2 operator /(const Float2& temp) const;

	Float2 operator +(float temp) const;
	Float2 operator -(float temp) const;
	Float2 operator *(float temp) const;
	Float2 operator /(float temp) const;
	
	// 代入演算子オーバーロード
	Float2& operator =(const Float2& temp);


	// ベクトルの機能
	float Dot(const Float2& temp) const;
	float Cross(const Float2& temp) const;
	float Length() const;
	void Normalize();

public: // メンバ変数
	float x, y;
};

