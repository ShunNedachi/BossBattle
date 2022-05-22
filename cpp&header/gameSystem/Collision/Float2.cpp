#include "Float2.h"
#include<math.h>

Float2 Float2::operator+(const Float2& temp)
{
	return Float2(x + temp.x, y + temp.y);
}

Float2 Float2::operator-(const Float2& temp) const
{
	return Float2(x - temp.x, y - temp.y);
}

Float2 Float2::operator*(const Float2& temp) const
{
	return Float2(x * temp.x, y * temp.y);
}

Float2 Float2::operator/(const Float2& temp) const
{
	return Float2(x / temp.x, y / temp.y);
}

Float2 Float2::operator+(float temp) const
{
	return Float2(x + temp, y + temp);
}

Float2 Float2::operator-(float temp) const
{
	return Float2(x - temp, y - temp);
}

Float2 Float2::operator*(float temp) const
{
	return Float2(x * temp, y * temp);
}

Float2 Float2::operator/(float temp) const
{
	return Float2(x / temp, y / temp);
}

Float2& Float2::operator=(const Float2& temp)
{
	x = temp.x;
	y = temp.y;
	return *this;
}

// �x�N�g���̋@�\

// ���ʂ�cos��
float Float2::Dot(const Float2& temp) const
{
	return x * temp.x + y * temp.y;
}
// ���ʂ�sin��
float Float2::Cross(const Float2& temp) const
{
	return x * temp.y - y * temp.x;
}

float Float2::Length() const
{
	return sqrt(x * x + y * y);
}

void Float2::Normalize()
{
	x = x / Length();
	y = y / Length();
}