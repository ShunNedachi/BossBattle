#include "Float3.h"
#include<math.h>

Float3 Float3::operator+(const Float3& temp) const
{
    return Float3(x + temp.x, y + temp.y, z + temp.z);
}
Float3 Float3::operator-(const Float3& temp) const
{
    return Float3(x - temp.x, y - temp.y, z - temp.z);
}
Float3 Float3::operator*(const Float3& temp) const
{
    return Float3(x * temp.x, y * temp.y, z * temp.z);
}
Float3 Float3::operator/(const Float3& temp) const
{
    return Float3(x / temp.x, y / temp.y, z / temp.z);
}

Float3 Float3::operator+(float temp) const
{
    return Float3(x + temp, y + temp, z + temp);
}
Float3 Float3::operator-(float temp) const
{
    return Float3(x - temp, y - temp, z - temp);
}
Float3 Float3::operator*(float temp) const
{
    return Float3(x * temp, y * temp, z * temp);
}
Float3 Float3::operator/(float temp) const
{
    return Float3(x / temp, y / temp, z / temp);
}

Float3& Float3::operator=(const Float3& temp)
{
    x = temp.x;
    y = temp.y;
    z = temp.z;
    return *this;
}

float Float3::Dot(const Float3& temp) const
{
    return x * temp.x + y * temp.y + z * temp.z;
}
Float3 Float3::Cross(const Float3& temp) const
{
    return Float3(y + temp.z - z * temp.y, z * temp.x - x * temp.z, x * temp.y - y * temp.x);
}

float Float3::Length() const
{
    return sqrt(x * x + y * y + z * z);
}

void Float3::Normalize()
{
    x /= Length();
    y /= Length();
    z /= Length();
}
