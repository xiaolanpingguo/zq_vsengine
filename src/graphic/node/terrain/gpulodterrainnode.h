#pragma once
#include "graphic/node/terrain/terrainnode.h"
#include "graphic/render/texture/texallstate.h"
namespace zq
{
	class GRAPHIC_API VSGPULodTerrainNode : public VSTerrainNode
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:

		VSGPULodTerrainNode();
		virtual ~VSGPULodTerrainNode();
	public:
// 		inline void SetDLodExtend(int32 uiDLodExtend)
// 		{
// 			m_uiDLodExtend = uiDLodExtend;
// 		}
// 		inline int32 GetDLodExtend()const
// 		{
// 			return m_uiDLodExtend;
// 		}
// 
// 		inline void SetDLodScale(float fDLodScale)
// 		{
// 			m_fDLodScale = fDLodScale;
// 		}
// 
// 		inline float GetDLodScale()const { return m_fDLodScale; }

	protected:
// 		int32 m_uiDLodExtend;
// 		float m_fDLodScale;
		VSTexAllStatePtr m_LodTexture;
		virtual bool CreateChild();
		virtual void UpdateNodeAll(double dAppTime);
	public:
		virtual bool CreateTerrain(uint32 uiNumX, uint32 uiNumZ, uint32 uiTessellationLevel);
	};
	DECLARE_Ptr(VSGPULodTerrainNode);
	VSTYPE_MARCO(VSGPULodTerrainNode);


}
