#pragma once
#include "graphic/core/reference.h"
#include "graphic/node/dynamicbuffergeometry.h"
namespace zq
{
	class GRAPHIC_API VSDebugDraw : public VSReference,public MemObject
	{
		//Graph Debug
	public:
		VSDebugDraw();
		~VSDebugDraw();
		void AddDebugLine(const Vector3 & P1,const Vector3 & P2,const uint32 &Color,bool bDepth);
		void AddDebugTriangle(const Vector3 & P1,const Vector3 & P2,const Vector3 &P3,
			const uint32 &Color,bool bDepth);
		void AddDebugLineAABB(const AABB & AABB,const uint32 &Color,bool bDepth);
		void AddDebugLineSphere(const Sphere & Sphere,const uint32 &Color,bool bDepth);
		void AddDebugLineOBB(const OBB & OBB,const uint32 &Color,bool bDepth);

		void AddDebugTriangleAABB(const AABB & AABB,const uint32 &Color,bool bDepth);
		void AddDebugTriangleSphere(const Sphere & Sphere,const uint32 &Color,bool bDepth);
		void AddDebugTriangleOBB(const OBB & OBB,const uint32 &Color,bool bDepth);
		bool m_bEnable;
	protected:
		struct DebugVertexType
		{
			Vector3 Pos;
			uint32 Color;

		};
		VSArray<DebugVertexType> DepthDebugLineArray;
		VSArray<DebugVertexType> NoDepthDebugLineArray;

		VSMaterialRPtr m_pOnlyVertexColor;
		VSMaterialRPtr m_pOnlyVertexColorDisableDepth;
		VSVertexFormatPtr m_pDrawVertexFormat;
	public:
		void DrawDebugInfo(VSCamera * pCamera);
	};
	DECLARE_Ptr(VSDebugDraw);
}	
