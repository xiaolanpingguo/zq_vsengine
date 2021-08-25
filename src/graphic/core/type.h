#pragma once


#include "graphic/core/pointer.h"
//#include "VSString.h"
//#include "datastruct/VSArray.h"
//#include "VSMap.h"
#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector3w.h"
#include "math/matrix3X3.h"
#include "math/matrix3X3w.h"
#include "math/quat.h"
#include "math/aabb.h"
#include "math/sphere.h"
#include "math/obb.h"
//#include "math/rect2.h"
#include "math/line.h"
#include "math/ray.h"
#include "math/segment.h"
#include "math/rectangle.h"
#include "math/triangle.h"
#include "math/plane.h"
#include "graphic/core/transform.h"

#include "datastruct/VSString.h"
#include "datastruct/VSArray.h"
#include "datastruct/VSBinaryTree.h"
#include "datastruct/VSMap.h"
#include "datastruct/VSBitArray.h"
#include "datastruct/VSDelegate.h"
#include "datastruct/VSGraph.h"
#include "datastruct/VSHash.h"
#include "datastruct/VSList.h"
#include "datastruct/VSMatch.h"

//һ���ڲ�ʹ�õĴ�object��̳еģ���Ҫ�������Եģ���������rtti���������ԣ�VSTYPE_MARCO
//���������ڲ���object��̳еģ�֧��������Ϊ��λ��ʵ�����л�

//�Զ������ͣ������Ҫ֧���Զ�������л���Ҫ��CUSTOMTYPE_MARCO  ���������������ԣ�
//����������԰汾�����Լ�Ҫ�ƶ���


namespace zq{


#define DEFINE_TYPE(TypeName)\
	template<typename T> struct TIs##TypeName##Type { enum { Value = false }; };\
	template<typename T> struct TIs##TypeName##PointerType { enum { Value = false }; };\
	template<typename T> struct TIs##TypeName##SmartPointerType { enum { Value = false }; };
#define DEFINE_TYPE_INSTANCE(TypeName,ClassName)\
	class ClassName;\
	template<> struct TIs##TypeName##Type<ClassName> { enum { Value = true }; }; \
	template<> struct TIs##TypeName##PointerType<ClassName *> { enum { Value = true }; }; \
	template<> struct TIs##TypeName##PointerType<const ClassName *> { enum { Value = true }; }; \
	template<> struct TIs##TypeName##SmartPointerType<VSPointer<ClassName>> { enum { Value = true }; }; \
	template<> struct TIs##TypeName##SmartPointerType<const VSPointer<ClassName>> { enum { Value = true }; };

	//custom type
	DEFINE_TYPE(Custom)
#define CUSTOMTYPE_MARCO(ClassName) \
	DEFINE_TYPE_INSTANCE(Custom,ClassName)

	// vs type
	DEFINE_TYPE(VS)
#define VSTYPE_MARCO(ClassName) \
	DEFINE_TYPE_INSTANCE(VS,ClassName)

	template<typename T> struct TIsVSEnumType									{ enum { Value = false }; };
#define VSENUMTYPE_MARCO(ClassName) \
	template<>	struct TIsVSEnumType<ClassName>									{ enum { Value = true }; }; 
	
	//cache resource
	DEFINE_TYPE(VSCacheResource)
#define DECLARE_CACHE_RESOURCE_TYPE(ClassName) \
	DEFINE_TYPE_INSTANCE(VSCacheResource,ClassName)

	DECLARE_CACHE_RESOURCE_TYPE(VSTextureCache)
	DECLARE_CACHE_RESOURCE_TYPE(VSShaderMapCache)

	template<typename T> struct TIsVSResourceProxyType							{ enum { Value = false}; };
	template<typename T> struct TIsVSResourceProxyPointerType					{ enum { Value = false}; };
	template<class T>
	class VSResourceProxy;
	template<typename T> struct TIsVSResourceProxyType<VSResourceProxy<T>>							{ enum { Value = true }; };
	template<typename T> struct TIsVSResourceProxyPointerType<VSPointer<VSResourceProxy<T>>>		{ enum { Value = true }; };


	
	// string
	template<typename T> struct TIsVSStringType									{ enum { Value = false}; };
	template<>	struct TIsVSStringType<VSString>								{ enum { Value = true }; };

	// name
	template<typename T> struct TIsVSUsedNameType								{ enum { Value = false }; };
	class VSUsedName;
	template<>	struct TIsVSUsedNameType<VSUsedName>							{ enum { Value = true }; };

	// Archive on need Loop VSArray 
	template<typename T> struct TIsNoNeedLoop
	{
		enum { Value = IS_ENUM(T) || TIsArithmeticType<T>::Value };	
	};
	template<>	struct TIsNoNeedLoop<Vector3W>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop<Vector3>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop<Vector2>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop<Matrix3X3W>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop<Matrix3X3>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <Quat>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <AABB>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <Sphere>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <OBB>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <Line>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <Ray>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <Segment>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <Rectangle>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <Triangle>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <Plane>									{ enum { Value = true }; };
	template<typename T> struct TIsNeedGC										
	{ 
		enum 
		{
			Value = TIsVSPointerType<T>::Value || 
			TIsVSSmartPointerType<T>::Value || 
			TIsCustomType<T>::Value || 
			TIsVSType<T>::Value
		};
	};

	template<typename T> struct TIsNeedGC<VSArray<T>>
	{
		enum
		{
			Value = TIsNeedGC<T>::Value
		};
	};

	template<typename Key, typename Value> struct TIsNeedGC<VSMap<Key, Value>>
	{
		enum
		{
			Value = TIsNeedGC<Key>::Value || TIsNeedGC<Value>::Value
		};
	};
}
