#pragma once
#include "core/memorymanager.h"
#include "graphic/core/object.h"
#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector3w.h"
#include "math/matrix3X3.h"
#include "math/matrix3X3w.h"
namespace zq
{
	class GRAPHIC_API VSVariant : public MemObject
	{
	public:
		enum 
		{
			V_NULL,
			V_BOOL,
			V_INT,
			V_REAL,
			V_VECTOR2,
			V_VECTOR3,
			V_VECTOR3W,
			V_MATRIX3X3,
			V_MATRIX3X3W,
			V_STRING,
			V_OBJECT,
			V_MAX
		};
		VSVariant(uint32 Type);
		~VSVariant();
		VSVariant(bool b);
		VSVariant(int32 i);
		VSVariant(float f);
		VSVariant(const Vector2 & v);
		VSVariant(const Vector3 & v);
		VSVariant(const Vector3W & v);
		VSVariant(const Matrix3X3 & m);
		VSVariant(const Matrix3X3W & m);
		VSVariant(VSObject * p);
		VSVariant(const char * s);
		VSVariant(const VSVariant & Va);

		VSVariant &operator=(const VSVariant &Va);
		VSVariant &operator=(bool b);
		VSVariant &operator=(int32 i);
		VSVariant &operator=(float f);
		VSVariant &operator=(const Vector2 & v);
		VSVariant &operator=(const Vector3 & v);
		VSVariant &operator=(const Vector3W & v);
		VSVariant &operator=(const Matrix3X3 & m);
		VSVariant &operator=(const Matrix3X3W & m);
		VSVariant &operator=(VSObject * p);
		VSVariant &operator=(const char * s);

		bool operator==(const VSVariant& rhs) const;
		bool operator==(bool b)const;
		bool operator==(int32 i)const;
		bool operator==(float f)const;
		bool operator==(const Vector2 & v)const;
		bool operator==(const Vector3 & v)const;
		bool operator==(const Vector3W & v)const;
		bool operator==(const Matrix3X3 & m)const;
		bool operator==(const Matrix3X3W & m)const;
		bool operator==(const VSObject * p)const;
		bool operator==(const char * s)const;

		uint32 GetType() const;
		bool IsValid()const;
		bool GetBool() const;
		int32 GetInt() const;
		float GetReal() const;
		const Vector2& GetVector2()const;
		const Vector3& GetVector3() const;
		const Vector3W& GetVector3W() const;
		const Matrix3X3& GetMatrix3X3() const;
		const Matrix3X3W& GetMatrix3X3W() const;
		const char* GetString()const;
		VSObject* GetObjectPtr()const;

		void Clear();

		bool GetValueToString(VSString& outVal)const;
		bool SetValueFromString(const VSString& inVal);
		static uint32 MapVariantType(const VSString& inType);
		static VSString MapVariantString(uint32 inType);
		
	private:
		uint32 m_uiType;
		union
		{
			bool m_bB;
			int32 m_iI;
			float m_fVec[16];
			void * m_pP;
		};
		static char *  ms_VariantMap[V_MAX];
	};
}
