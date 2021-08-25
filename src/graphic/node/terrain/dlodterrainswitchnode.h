#pragma once
#include "graphic/node/switchnode.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSDLodTerrainSwitchNode : public VSSwitchNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSDLodTerrainSwitchNode(uint32 uiIndexXInTerrain,uint32 uiIndexZInTerrain);
		virtual ~VSDLodTerrainSwitchNode();

	public:

		virtual void UpdateView(VSCuller & Culler,double dAppTime);
		
	
	protected:
		VSDLodTerrainSwitchNode();
		uint32 m_uiIndexXInTerrain;
		uint32 m_uiIndexZInTerrain;
	};
	DECLARE_Ptr(VSDLodTerrainSwitchNode);
	VSTYPE_MARCO(VSDLodTerrainSwitchNode);
}
