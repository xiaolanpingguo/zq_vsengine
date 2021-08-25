#pragma once
#include "graphic/node/terrain/terrainnode.h"
namespace zq
{
	class GRAPHIC_API VSDLodTerrainNode : public VSTerrainNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		
		VSDLodTerrainNode();
		virtual ~VSDLodTerrainNode();
	public:
		inline void SetDLodExtend(int32 uiDLodExtend)
		{
			m_uiDLodExtend = uiDLodExtend;
		}
		inline int32 GetDLodExtend()const
		{
			return m_uiDLodExtend;
		}

		inline void SetDLodScale(float fDLodScale)
		{
			m_fDLodScale = fDLodScale;
		}

		inline float GetDLodScale()const { return m_fDLodScale; }
		
	protected:
		int32 m_uiDLodExtend;
		float m_fDLodScale;
		virtual bool CreateChild();
		virtual void UpdateNodeAll(double dAppTime);
	public:
		
	};
	DECLARE_Ptr(VSDLodTerrainNode);
	VSTYPE_MARCO(VSDLodTerrainNode);


}
