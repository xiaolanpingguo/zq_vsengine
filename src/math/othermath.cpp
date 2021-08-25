#include "math/othermath.h"


namespace zq{


bool IsUniformScale(float fScale)
{
    if (ABS(fScale - 1.0f) < EPSILON_E4)
    {
        return true;
    }
    return false;
}
bool IsZeroTranslate(const Vector3 & Translate)
{
    if (Translate.GetSqrLength() < EPSILON_E4)
    {
        return true;
    }
    return false;
}
bool IsIdentityRotate(const Matrix3X3 & Rotate)
{
    for (uint32 i = 0; i < 3; i++)
    {
        for (uint32 j = 0; j < 3; j++)
        {
            if (i != j)
            {
                if (ABS(Rotate.M[i][j]) > EPSILON_E4)
                {
                    return false;
                }
            }
            else
            {
                if (ABS(Rotate.M[i][j] - 1.0f) > EPSILON_E4)
                {
                    return false;
                }
            }

        }
    }
    return true;
}
bool IsIdentityRotate(const Quat & Rotate)
{
    Vector3 R(Rotate.x, Rotate.y, Rotate.z);
    if (R.GetSqrLength() < EPSILON_E4)
    {
        return true;
    }
    return false;
}
float LineInterpolation(float t1, float t2, float t)
{
    return t1 + (t2 - t1) * t;
}
Vector3 LineInterpolation(const Vector3& t1, const Vector3& t2, float t)
{
    return t1 + (t2 - t1) * t;
}
Quat LineInterpolation(const Quat& t1, const Quat& t2, float t)
{
    float fCos = t2.Dot(t1);
    Quat Temp = t1;
    if (fCos < 0.0f)
    {
        Temp = t1 * (-1.0f);
    }
    return Temp + (t2 - Temp) * t;
}
Vector3W LineInterpolation(const Vector3W& t1, const Vector3W& t2, float t)
{
    return t1 + (t2 - t1) * t;
}
}
