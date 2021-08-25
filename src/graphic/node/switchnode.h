#pragma once
#include "graphic/node/node.h"
namespace zq
{
	class GRAPHIC_API VSSwitchNode : public VSNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSSwitchNode();
		virtual ~VSSwitchNode();


		virtual VSSpatial * GetActiveNode()const;
	protected:
		uint32 m_uiActiveNode;

	protected:
		virtual void UpdateWorldBound(double dAppTime);//更新世界边界体 

		virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
	};
	DECLARE_Ptr(VSSwitchNode);
	VSTYPE_MARCO(VSSwitchNode);

}
