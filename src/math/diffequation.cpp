#include "math/diffequation.h"


namespace zq{


DiffEquation::DiffEquation()
{
	m_pFunc = NULL;
	m_Step = 0.0;
	m_BeginT = 0.0;
	m_BeginX = 0.0;
}

DiffEquation::~DiffEquation()
{
	m_pFunc = NULL;
	m_Step = 0.0;
	m_BeginT = 0.0;
	m_BeginX = 0.0;
}

void DiffEquation::Update()
{
	VSMAC_ASSERT(m_pFunc);

	float HalfStep = m_Step / (float)2.0;
	float T = m_BeginT + HalfStep;
	
	float X = m_BeginX + m_pFunc->GetFirstDerivative(m_BeginT) * HalfStep;

	m_BeginX = X + m_pFunc->GetFirstDerivative(T) * HalfStep;

	m_BeginT +=m_Step;
}

}