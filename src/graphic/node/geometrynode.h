#pragma once
#include "graphic/node/node.h"
#include "graphic/controller/morph/morphset.h"
#include "graphic/node/geometry.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSGeometryNode : public VSNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSGeometryNode();
		virtual ~VSGeometryNode();
		void SetMorphSet(VSMorphSet * pMorphSet);
		inline const VSMorphSet *GetMorphSet()const
		{
			return m_pMorphSet;
		}
		VSGeometry * GetGeometry(uint32 i)const;
		VSGeometry * GetNormalGeometry(uint32 index)const;
		uint32 GetNormalGeometryNum()const;
		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
		void SetMorphData(const VSUsedName & ShowName, float fData);
		void SetMorphData(uint32 Index, float fData);
		float m_fLODScreenSize = 1.0f;
		virtual void GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const;
	protected:
		VSMorphSetPtr m_pMorphSet;
		void UpdateLocalAABB();
	};
	DECLARE_Ptr(VSGeometryNode);
	VSTYPE_MARCO(VSGeometryNode);
}
