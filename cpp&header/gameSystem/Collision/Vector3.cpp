#include "Vector3.h"

Vector3& Vector3::operator=(const Float3& temp)
{
    // バグっていたら直す
    x = temp.x;
    y = temp.y;
    z = temp.z;
    return *this;
}
