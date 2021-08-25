#pragma once
#include "graphic/node/meshnode.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSTerrainNode : public VSMeshNode
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum
		{
			MAX_TESSELLATION_LEVEL = 8,
			MIN_TESSELLATION_LEVEL = 1,
			MAX_GPU_TESSELLATION_LEVEL = 6,
			MAX_NUM = 10,
			MIN_NUM = 1,
			WIDTH_SCALE = 50
		};

		virtual ~VSTerrainNode() = 0;
	protected:
		VSTerrainNode();
		uint32 m_uiTessellationLevel;		//每个儿子层级，儿子几点个数和网格密度关系看GetTileNumX函数
		uint32 m_uiNumX;					//网格密度
		uint32 m_uiNumZ;
		uint32 m_uiTotalNum;

		unsigned char *m_pHeight;
		float m_fHeightScale;
		virtual bool CreateChild() = 0;
		void SetNum(uint32 uiNumX,uint32 uiNumZ);
		void SetTessellationLevel(uint32 uiTessellationLevel, uint32 uiMaxTessellationLevel = MAX_TESSELLATION_LEVEL, 
			uint32 uiMinTessellationLevel = MIN_TESSELLATION_LEVEL);
	public:
		inline uint32 GetTessellationLevel()const { return m_uiTessellationLevel ;}
		inline uint32 GetNumX()const{return m_uiNumX;}
		inline uint32 GetNumZ()const{return m_uiNumZ;}
		inline uint32 GetTileNumX()const {return 1<<(m_uiNumX - m_uiTessellationLevel);}
		inline uint32 GetTileNumZ()const {return 1<<(m_uiNumZ - m_uiTessellationLevel);}
		inline uint32 GetSizeX()const{ return (1 << m_uiNumX) + 1; }
		inline uint32 GetSizeZ()const{ return (1 << m_uiNumZ) + 1; }
		
		
		virtual bool CreateTerrain(uint32 uiNumX,uint32 uiNumZ,uint32 uiTessellationLevel);
		bool CreateTerrainFromHeightMap(const char *pFileName,uint32 uiTessellationLevel,float fHeightScale);
		
		inline float GetHeightScale()const
		{
			return m_fHeightScale;
		}
		unsigned char GetRawHeight(uint32 uiX,uint32 uiZ);
		float GetHeight(uint32 uiX,uint32 uiZ);
	};
	DECLARE_Ptr(VSTerrainNode);
	VSTYPE_MARCO(VSTerrainNode);
}
