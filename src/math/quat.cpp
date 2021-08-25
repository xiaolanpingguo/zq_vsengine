#include "math/maths.h"
#include "math/quat.h"
#include "math/vector3.h"
#include "math/matrix3X3.h"


namespace zq{


Quat::Quat(void) 
{ 
	x=0.0f, y=0.0f, z=0.0f, w=1.0f; 
}
/*----------------------------------------------------------------*/
Quat::Quat(float _x, float _y, float _z, float _w)
{ 
	x=_x; y=_y; z=_z; w=_w; 
}
/*----------------------------------------------------------------*/
void Quat::Set(float _x, float _y, float _z, float _w)
{
	x=_x; y=_y; z=_z; w=_w; 
}
/*----------------------------------------------------------------*/
void Quat::CreateEuler(float fRoll, float fPitch, float fYaw)
{
	float cX, cY, cZ, sX, sY, sZ, cXcZ, sXsZ, cXsZ, sXcZ;

	fPitch *= 0.5f;
	fYaw   *= 0.5f;
	fRoll  *= 0.5f;

	cX = COS(fPitch);
	cY = COS(fYaw);
	cZ = COS(fRoll);

	sX = SIN(fPitch);
	sY = SIN(fYaw);
	sZ = SIN(fRoll);

	cXcZ = cX * cZ;
	sXsZ = sX * sZ;
	cXsZ = cX * sZ;
	sXcZ = sX * cZ;
	
	w = cXcZ * cY + sXsZ * sY;
	x = sXcZ * cY + cXsZ * sY;
	y = cXcZ * sY - sXsZ * cY;
	z = cXsZ * cY - sXcZ * sY;

} // MakeFromEuler
/*----------------------------------------------------------------*/
void Quat::Normalize(void) 
{

	float m=SQRT(x*x + y*y + z*z + w*w);
	float Invm = 1.0f/ m;
	x *= Invm; y *= Invm; z *= Invm; w *= Invm;
}
/*----------------------------------------------------------------*/
Quat Quat::GetConjugate()const
{
	
	return Quat(-x,-y,-z,w);
}
/*----------------------------------------------------------------*/
void Quat::GetEulers(float &fRoll, float &fPitch, float &fYaw)const
{

	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;


	x2 = x + x;
	y2 = y + y;
	z2 = z + z;

	xx = x * x2;
	xy = x * y2;
	xz = x * z2;

	yy = y * y2;
	yz = y * z2;
	zz = z * z2;

	wx = w * x2;
	wy = w * y2;
	wz = w * z2;

	float _00 = 1.0f - (yy + zz);//
	float _01 = xy + wz;//


	float _10 = xy - wz;//
	float _11 = 1.0f - (xx + zz);//


	float _20 = xz + wy;//
	float _21 = yz - wx;//
	float _22 = 1.0f - (xx + yy);//

	if (_21 > 1.0f)
		fPitch = -MATH_PI2;
	else if (_21 < -1.0f)
		fPitch = MATH_PI2;
	else
		fPitch = ASIN(-_21);

	if (fabs(_21) > MATH_FLOAT_1)
	{
		fYaw = 0;
		fRoll = ATAN2(-_10, _00);

	}
	else
	{
		fRoll = ATAN2(_01, _11);
		fYaw = ATAN2(_20, _22);

	}


} // EulerFromQuat
/*----------------------------------------------------------------*/
void Quat::GetMatrix(Matrix3X3 &Mat)const
{
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;


	x2 = x + x; 
	y2 = y + y; 
	z2 = z + z;

	xx = x * x2;   
	xy = x * y2;  
	xz = x * z2;

	yy = y * y2;   
	yz = y * z2;   
	zz = z * z2;

	wx = w * x2;   
	wy = w * y2;   
	wz = w * z2;

	Mat._00 = 1.0f - (yy + zz);
	Mat._01 = xy + wz;
	Mat._02 = xz - wy;

	Mat._10 = xy - wz;
	Mat._11 = 1.0f - (xx + zz);
	Mat._12 = yz + wx;

	Mat._20 = xz + wy;
	Mat._21 = yz - wx;
	Mat._22 = 1.0f - (xx + yy);
} // GetMatrix
/*----------------------------------------------------------------*/
float Quat::GetLength(void)const 
{

	return SQRT(x*x + y*y + z*z + w*w);

}
/*----------------------------------------------------------------*/
void Quat::operator /= (float f) 
{ 

	w /= f;  x /= f;  y /= f;  z /= f; 

}

/*----------------------------------------------------------------*/
Quat Quat::operator / (float f) const
{

	return Quat(x/f, y/f, z/f, w/f);

}
/*----------------------------------------------------------------*/

void Quat::operator *= (float f) 
{ 
	w *= f;  x *= f;  y *= f;  z *= f; 
}
/*----------------------------------------------------------------*/
Quat Quat::operator * (float f) const
{

	return Quat(x*f, y*f, z*f, w*f);

}
/*----------------------------------------------------------------*/


void Quat::operator += (const Quat &q) 
{ 

	w += q.w;  x += q.x;  y += q.y;  z += q.z; 

}
/*----------------------------------------------------------------*/
Quat Quat::operator + (const Quat &q) const
{

	return Quat(x+q.x, y+q.y, z+q.z, w+q.w);

}
/*----------------------------------------------------------------*/
void Quat::operator -= (const Quat &q) 
{ 

	w -= q.w;  x -= q.x;  y -= q.y;  z -= q.z; 

}
/*----------------------------------------------------------------*/
Quat Quat::operator - (const Quat &q) const
{
	
	return Quat(x-q.x, y-q.y, z-q.z, w-q.w);

}
/*----------------------------------------------------------------*/
void Quat::operator *= (const Quat &q) 
{

	w = w * q.w - x * q.x - y * q.y - z * q.z;
	x = w * q.x + x * q.w + z * q.y - y * q.z;
	y = w * q.y + y * q.w + x * q.z - z * q.x;
	z = w * q.z + z * q.w + y * q.x - x * q.y;

}
/*----------------------------------------------------------------*/


Quat Quat::operator * (const Quat &q) const 
{
	Quat qResult;

	qResult.w = w * q.w - x * q.x - y * q.y - z * q.z;
	qResult.x = w * q.x + x * q.w + z * q.y - y * q.z;
	qResult.y = w * q.y + y * q.w + x * q.z - z * q.x;
	qResult.z = w * q.z + z * q.w + y * q.x - x * q.y;

	return qResult;
}
/*----------------------------------------------------------------*/
Quat Quat::operator * (const Vector3 &v) const 
{
	Quat t(v.x, v.y, v.z, 0);
	return (*this) * t;
}
/*----------------------------------------------------------------*/
bool Quat::operator ==(const Quat &q)const
{
	for (uint32 i = 0 ; i < 4 ; i++)
	{
		if (ABS(m[i] - q.m[i]) > EPSILON_E4)
		{
			return false;
		}
	}
	return true;
}
/*----------------------------------------------------------------*/
void Quat::Rotate(const Quat &q1, const Quat &q2) 
{ 
	Quat m1 = q1;
	Quat m2 = q2;
	m1.Normalize();
	m2.Normalize();
	Quat t = (~m1)*m2*m1;
	x=t.x; y=t.y; z=t.z; w=t.w; 
}
/*----------------------------------------------------------------*/
Vector3 Quat::Rotate(const Vector3 &v)const 
{ 
	Quat t(x, y, z, w);
	t.Normalize();
	Quat r = (~t)*v*t;
	return Vector3(r.x, r.y, r.z); 
}
/*----------------------------------------------------------------*/
float Quat::Dot(const Quat& q)const
{

	return (w * q.w + x * q.x + y * q.y + z * q.z);
}
/*----------------------------------------------------------------*/
Quat Quat::operator ~(void) const
{ 
	return Quat(-x, -y, -z, w); 
}
/*----------------------------------------------------------------*/
Quat Quat::GetInverse()const
{
	return (GetConjugate()/GetLength());

}
/*----------------------------------------------------------------*/
void Quat::CreateAxisAngle(const Vector3& Axis,float fAngle)
{
	float fTemp = fAngle/2.0f;
	float Sin = SIN(fTemp);
	float Cos = COS(fTemp);
	
	Vector3 N = Axis;
	N.Normalize();
	
	w = Cos;
	x = Sin * N.x;
	y = Sin * N.y;
	z = Sin * N.z;
}
/*----------------------------------------------------------------*/
void Quat::GetAxisAngle(Vector3 & Axis , float & fAngle)const
{

	Quat QTemp = *this;
	float fHalfAngle = ACOS(QTemp.w);

	fAngle = fHalfAngle * 2.0f;

	float SinHalfAngle = 1.0f - QTemp.w * QTemp.w;

	float InvSinHalfAngle = 1.0f / SinHalfAngle;
	Axis.x = QTemp.x * InvSinHalfAngle;
	Axis.y = QTemp.y * InvSinHalfAngle;
	Axis.z = QTemp.z * InvSinHalfAngle;


}
/*----------------------------------------------------------------*/
void Quat::Slerp(float t,const Quat & q1,const Quat & q2)
{
	if(t <= 0)  
	{
		(*this) = q1;
		return ;
	}
	if(t >= 1)  
	{
		(*this) = q2;
		return;
	}

	

	Quat q1Temp,q2Temp;
	q1Temp = q1;
	q2Temp = q2;
	q1Temp.Normalize();
	q2Temp.Normalize();
	float fCos = q1Temp.Dot(q2Temp);
	if(fCos < 0.0f)
	{
		q1Temp *= -1.0f;
		fCos = -fCos;
	}
	float k1 ,k2;
	if(fCos > MATH_FLOAT_1)
	{
		k1 = 1.0f - t;
		k2 = t;
	}
	else
	{
		float fSin = SQRT(1.0f - fCos * fCos);
		float fAngle = ATAN2(fSin, fCos);
		float fInvSin = 1.0f / fSin;
		k1 = SIN((1.0f - t) * fAngle) * fInvSin;
		k2 = SIN(t * fAngle) * fInvSin;

	
	}
	x = k1 * q1Temp.x + k2 * q2Temp.x;
	y = k1 * q1Temp.y + k2 * q2Temp.y;
	z = k1 * q1Temp.z + k2 * q2Temp.z;
	w = k1 * q1Temp.w + k2 * q2Temp.w;


}
/*----------------------------------------------------------------*/
Quat Quat::Pow(float exp)const
{
	if(ABS(w) > MATH_FLOAT_1)
		return *this;

	float alpha = ACOS(w);
	float newAlpha = alpha * exp;

	float mult = SIN(newAlpha) / SIN(alpha);
	return Quat(x * mult , y * mult , z * mult,COS(newAlpha)); 
}
/*----------------------------------------------------------------*/
Quat Quat::Ln()const
{
	if(ABS(w) > MATH_FLOAT_1)
		return Quat(x , y , z , 0.0f);;
	float alpha = ACOS(w);

	float Sin = SIN(alpha);
	float temp = alpha / Sin;
	return Quat( temp * x  , temp * y ,temp * z  , 0.0f);
	
}
/*----------------------------------------------------------------*/
Quat Quat::Exp()const
{
	float alpha = GetLength();
	float Sin = SIN(alpha);
	if(ABS(Sin) > EPSILON_E4)
	{
		float temp = Sin / alpha;
		return Quat(x * temp , y * temp , z * temp,COS(alpha));
	
	}
	else
	{
		return Quat(x  , y , z ,COS(alpha));
	}
}
/*----------------------------------------------------------------*/
void Quat::SlerpSValueOf(const Quat & q1,const Quat & q2,const Quat & q3)
{
	Quat q2Inv = q2.GetInverse();
	Quat K1 = q1 * q2;
	Quat K2 = q3 * q2;

	Quat K3 = ( K1.Ln() + K2.Ln() ) * (-0.25f);
	*this = q2 * K3.Exp();
	

}
/*----------------------------------------------------------------*/
void Quat::Slerp(float t,const Quat & q1,const Quat & q2,const Quat & s1,const Quat & s2)
{
	float fSlerpT = 2.0f * t * (1.0f - t);
	Quat SlerpP;
	SlerpP.Slerp(t,q1,q2);
	Quat SlerpQ;
	SlerpQ.Slerp(t,s1,s2);
	Slerp(fSlerpT,SlerpP,SlerpQ);

}
/*----------------------------------------------------------------*/
void Quat::TriangleSlerp(float t1,float t2, const Quat & q1,const Quat & q2,const Quat & q3)
{
	*this = q1.Pow(t1) + q2.Pow(t2) + q3.Pow(1.0f - t1 - t2);
}

}