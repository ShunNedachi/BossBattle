#include "Vector3.h"

Vector3& Vector3::operator=(const Float3& temp)
{
    // �o�O���Ă����璼��
    x = temp.x;
    y = temp.y;
    z = temp.z;
    return *this;
}
