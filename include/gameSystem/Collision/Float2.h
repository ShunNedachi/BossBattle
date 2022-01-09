#pragma once

// float2������N���X
class Float2
{
public: // �����o�֐�

	Float2():x(0),y(0) {}
	Float2(float x, float y):x(x),y(y) {}
	~Float2() {}
	
	// �l�����Z�p
	Float2 operator +(const Float2& temp);
	Float2 operator -(const Float2& temp)const;
	Float2 operator *(const Float2& temp)const;
	Float2 operator /(const Float2& temp) const;

	Float2 operator +(float temp) const;
	Float2 operator -(float temp) const;
	Float2 operator *(float temp) const;
	Float2 operator /(float temp) const;
	
	// ������Z�q�I�[�o�[���[�h
	Float2& operator =(const Float2& temp);


	// �x�N�g���̋@�\
	float Dot(const Float2& temp) const;
	float Cross(const Float2& temp) const;
	float Length() const;
	void Normalize();

public: // �����o�ϐ�
	float x, y;
};

