#include "graphic/node/terrain/terrainnode.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/config.h"
#include "graphic/core/stream/stream.h"

using namespace zq;
IMPLEMENT_RTTI_NoCreateFun(VSTerrainNode,VSMeshNode)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSTerrainNode)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSTerrainNode,VSMeshNode)
REGISTER_PROPERTY(m_uiTessellationLevel,TessellationLevel,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiNumX,NumX,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiNumZ,NumZ,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fHeightScale,HeightScale,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY_DATA(m_pHeight,m_uiTotalNum,HeightData);
END_ADD_PROPERTY

VSTerrainNode::VSTerrainNode()
{
	m_uiTessellationLevel = MIN_TESSELLATION_LEVEL;
	m_uiNumX = MIN_NUM;
	m_uiNumZ = MIN_NUM;

	m_pHeight = NULL;
	m_fHeightScale = 1.0f;
	m_uiTotalNum = 0;
}

VSTerrainNode::~VSTerrainNode()
{
	SAFE_DELETEA(m_pHeight);
}

void VSTerrainNode::SetTessellationLevel(uint32 uiTessellationLevel, uint32 uiMaxTessellationLevel,
	uint32 uiMinTessellationLevel)
{
	m_uiTessellationLevel = uiTessellationLevel;
	if(m_uiTessellationLevel > m_uiNumX)
	{
		m_uiTessellationLevel = m_uiNumX;
	}

	if(m_uiTessellationLevel > m_uiNumZ)
	{
		m_uiTessellationLevel = m_uiNumZ;
	}

	if (m_uiTessellationLevel > uiMaxTessellationLevel)
	{
		m_uiTessellationLevel = uiMaxTessellationLevel;
	}
	if (m_uiTessellationLevel < uiMinTessellationLevel)
	{
		m_uiTessellationLevel = uiMinTessellationLevel;
	}
}
void VSTerrainNode::SetNum(uint32 uiNumX,uint32 uiNumZ)
{
	m_uiNumX = uiNumX;
	m_uiNumZ = uiNumZ;
	if (m_uiNumX > MAX_NUM)
	{
		m_uiNumX = MAX_NUM;
	}
	if(m_uiNumX < MIN_NUM)
	{
		m_uiNumX = MIN_NUM;
	}
	
	if (m_uiNumZ > MAX_NUM)
	{
		m_uiNumZ = MAX_NUM;
	}
	if(m_uiNumZ < MIN_NUM)
	{
		m_uiNumZ = MIN_NUM;
	}
	if (m_uiTessellationLevel > m_uiNumX || m_uiTessellationLevel > m_uiNumZ)
	{
		SetTessellationLevel(m_uiTessellationLevel);
	}
	m_uiTotalNum = ((1 << m_uiNumX) + 1) * ((1 << m_uiNumZ) + 1);
}

bool VSTerrainNode::CreateTerrain(uint32 uiNumX,uint32 uiNumZ,uint32 uiTessellationLevel)
{
	
	SetNum(uiNumX,uiNumZ);
	SetTessellationLevel(uiTessellationLevel);
	SAFE_DELETEA(m_pHeight);
	m_pHeight = MEM_NEW unsigned char[m_uiTotalNum];
	if (!m_pHeight)
	{
		return 0;
	}
	memset(m_pHeight,0,m_uiTotalNum * sizeof(unsigned char));
	if (!CreateChild())
	{
		return 0;
	}
	m_bIsChanged = true;
	CreateLocalAABB();
	UpdateAll(0.0f);
	return 1;
}
bool VSTerrainNode::CreateTerrainFromHeightMap(const char *pFileName,uint32 uiTessellationLevel,float fHeightScale)
{
	VSMAC_ASSERT(pFileName);

	
	File *pFile = NULL;
	pFile = MEM_NEW File();
	VSMAC_ASSERT(pFile);

	uint32 uiHeightSize = 0;
	SAFE_DELETEA(m_pHeight);

	VSString TerrainPath = VSConfig::ms_TerrainPath + pFileName;
	if(!pFile->Open(TerrainPath.GetBuffer(),File::OM_RB))
	{
		SAFE_DELETE(pFile);
		return 0;
	}
	
	pFile->Read(&uiHeightSize, 1, sizeof(unsigned int));
	m_pHeight= MEM_NEW unsigned char [uiHeightSize * uiHeightSize];

	VSMAC_ASSERT(m_pHeight);


	pFile->Read(m_pHeight, 1, uiHeightSize * uiHeightSize);

	SAFE_DELETE(pFile);
	uiHeightSize = uiHeightSize - 1;
	uint32 uiNum = FastLog2(uiHeightSize);
	
	SetNum(uiNum,uiNum);
	SetTessellationLevel(uiTessellationLevel);
	m_fHeightScale = fHeightScale;
	if (!CreateChild())
	{
		return 0;
	}
	m_bIsChanged = true;
	CreateLocalAABB();
	UpdateAll(0.0f);
	return 1;
}

unsigned char VSTerrainNode::GetRawHeight(uint32 uiX,uint32 uiZ)
{
	uint32 uiLen = ((1 << m_uiNumX) + 1);
	if (m_pHeight && uiX < uiLen && uiZ < uiLen)
	{
		return m_pHeight[uiX * uiLen + uiZ];
	}
	return 0;
}
float VSTerrainNode::GetHeight(uint32 uiX,uint32 uiZ)
{
	uint32 uiLen = ((1 << m_uiNumX) + 1);
	if (m_pHeight && uiX < uiLen && uiZ < uiLen)
	{
		return m_pHeight[uiX * uiLen + uiZ] * m_fHeightScale;
	}
	return 0.0f;

}
