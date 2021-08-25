#include "math/surfacesubdivision.h"
#include "core/memorymanager.h"


namespace zq{


SurfaceSubdivision::SurfaceSubdivision()
{
	m_pSurface = NULL;
	m_pPoint = NULL;
	m_uiRow = 0;
	m_uiColumn = 0;
	m_pUV = NULL;
}

SurfaceSubdivision::~SurfaceSubdivision()
{
	SAFE_DELETEA(m_pPoint);
	SAFE_DELETEA(m_pUV);
	m_pSurface = NULL;
	m_uiRow = 0;
	m_uiColumn = 0;
}





//----------------------------SurfaceSubdivisionByLength-----------------
SurfaceSubdivisionByLength::SurfaceSubdivisionByLength(uint32 Row, uint32 Column, float U1, float U2, float V1, float V2, uint32 uiIterations)
{
    m_uiRow = Row;
    m_uiColumn = Column;
    m_fU1 = U1;
    m_fU2 = U2;
    m_fV1 = V1;
    m_fV2 = V2;
    m_uiIterations = uiIterations;
}

SurfaceSubdivisionByLength::~SurfaceSubdivisionByLength()
{
}

bool SurfaceSubdivisionByLength::Subdivision()
{
    VSMAC_ASSERT(m_uiRow && m_uiColumn && m_pSurface);

    VSMAC_ASSERT(m_fU2 <= m_fU1 && m_fV2 <= m_fV1);

    return 1;
    // 	SAFE_DELETEA(m_pPoint);
    // 
    // 	m_pPoint = MEM_NEW Vector3[m_uiRow * m_uiColumn];
    // 	if(!m_pPoint)
    // 		return 0;
    // 
    // 	SAFE_DELETEA(m_pUV);
    // 	m_pUV = MEM_NEW Vector2[m_uiRow * m_uiColumn];
    // 	if(!m_pUV)
    // 		return 0;
    // 	float fi = (float)GetULength()/ (m_uiRow - 1);
    // 	float fj = (float)(m_fV2 - m_fV1) / (m_uiColumn - 1);
    // 	float fiTemp = m_fU1;
    // 	for(uint32 i = 0 ; i < m_uiRow ; i++)
    // 	{ 
    // 		float fjTemp = m_fV1;
    // 		uint32 Temp = i * m_uiColumn;
    // 		for(uint32 j = 0 ; j < m_uiColumn ; j++)
    // 		{
    // 			m_pUV[Temp + j] = Vector2(fiTemp,fjTemp);
    // 			m_pPoint[ Temp + j] = m_pSurface->GetPoint(fiTemp,fjTemp);
    // 			fjTemp += fj;
    // 		}
    // 		fiTemp += fi;
    // 	}
    // 	return 1;
    // 
    // 
    // 
    // 	SAFE_DELETEA(m_pT);
    // 	m_pT = MEM_NEW float[m_uiPointNum]; 
    // 
    // 	if(!m_pT)
    // 		return 0;
    // 
    // 	float det = m_pCurve->GetLength(m_ft1,m_ft2,m_uiIterations)/float(m_uiPointNum - 1);
    // 	float temp = m_ft1;
    // 	for(uint32 i = 0 ; i < m_uiPointNum ; i++)
    // 	{
    // 		m_pT[i] = m_pCurve->GetTime(temp,m_uiIterations);
    // 		m_pPoint[i] = m_pCurve->GetPoint(m_pT[i]);
    // 		temp += det;
    // 
    // 	}

    // 	return 1;
}





//----------------------------SurfaceSubdivisionByUniform-----------------
SurfaceSubdivisionByUniform::SurfaceSubdivisionByUniform(uint32 uiRow, uint32 uiColumn, float U1, float U2, float V1, float V2)
{

}

SurfaceSubdivisionByUniform::~SurfaceSubdivisionByUniform()
{

}

bool SurfaceSubdivisionByUniform::Subdivision()
{
    VSMAC_ASSERT(m_uiRow && m_uiColumn && m_pSurface);

    VSMAC_ASSERT(m_fU2 <= m_fU1 && m_fV2 <= m_fV1);
    SAFE_DELETEA(m_pPoint);

    m_pPoint = MEM_NEW Vector3[m_uiRow * m_uiColumn];
    VSMAC_ASSERT(m_pPoint);

    SAFE_DELETEA(m_pUV);
    m_pUV = MEM_NEW Vector2[m_uiRow * m_uiColumn];
    VSMAC_ASSERT(m_pUV);

    float fi = (float)(m_fU2 - m_fU1) / (m_uiRow - 1);
    float fj = (float)(m_fV2 - m_fV1) / (m_uiColumn - 1);
    float fiTemp = m_fU1;
    for (uint32 i = 0; i < m_uiRow; i++)
    {
        float fjTemp = m_fV1;
        uint32 Temp = i * m_uiColumn;
        for (uint32 j = 0; j < m_uiColumn; j++)
        {
            m_pUV[Temp + j] = Vector2(fiTemp, fjTemp);
            m_pPoint[Temp + j] = m_pSurface->GetPoint(fiTemp, fjTemp);
            fjTemp += fj;
        }
        fiTemp += fi;
    }

    return 1;
}

}