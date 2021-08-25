#pragma once
#include "graphic/node/node.h"
#include "graphic/core/resource.h"
namespace zq
{
	class VSGeometryNode;
	class VSSwitchNode;
	class VSStream;
	class GRAPHIC_API VSMeshNode : public VSNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:

		VSMeshNode();
		virtual ~VSMeshNode();
		inline void SetIsDrawBoundVolume(bool bIsDrawBoundVolume)
		{
			m_bIsDrawBoundVolume = bIsDrawBoundVolume;
		}
		inline uint32 GetRenderGroup()const
		{
			return m_uiRenderGroup;
		}
		inline void SetRenderGroup(uint32 uiRenderGroup)
		{
			m_uiRenderGroup = uiRenderGroup;
		}
		virtual bool SetDrawInstance(bool bDrawInstance) {return false;}
		virtual void UpdateView(VSCuller & Culler,double dAppTime);

		virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
		bool m_bReceiveShadow;
		bool m_bCastShadow;
		bool m_bLighted;
		inline bool GetDrawInstance()const
		{
			return m_bDrawInstance;
		}
	protected:
		bool m_bDrawInstance;
		bool m_bIsDrawBoundVolume;
		uint32 m_uiRenderGroup;
	};
	DECLARE_Ptr(VSMeshNode);
	VSTYPE_MARCO(VSMeshNode);
}
