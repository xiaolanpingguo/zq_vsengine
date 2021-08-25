#include "math/curve.h"
#include "math/matrix3X3w.h"


namespace zq{


Curve::Curve()
{

}

Curve::~Curve()
{


}
/*----------------------------------------------------------------*/
void Curve::GetFixedUpBNT(float t ,Vector3 & B,Vector3 & N,Vector3& T)
{
	Matrix3X3W Mat;
	Vector3 Point(0,0,0);
	Vector3 FD = GetFirstDerivative(t);
	FD.Normalize();
	Mat.CreateFromLookAt(Point,FD);
	B.x = Mat._00;
	B.y = Mat._10;
	B.z = Mat._20;

	N.x = Mat._01;
	N.y = Mat._11;
	N.z = Mat._21;

	T.x = Mat._02;
	T.y = Mat._12;
	T.z = Mat._22;
}

}