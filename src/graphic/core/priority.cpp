#include "graphic/core/priority.h"


namespace zq{


bool VSPriority::AddPriorityThan(VSPriority * pPriority)
{
	if(!pPriority)
		return 0;
	if(pPriority->CheckIsCircle(this))
		return 0;
	m_uiPriorityNum = 0;
	
	m_pPriorityThanList.AddElement(pPriority);
	return 1;
}

uint32 VSPriority::GetPriorityNum()
{
	CheckPriorityNum();
	return m_uiPriorityNum;
}

VSPriority::VSPriority()
{
	m_uiPriorityNum = 0;
}

VSPriority::VSPriority(uint32 uiPriorityNum)
{
	m_uiPriorityNum = uiPriorityNum;
}

VSPriority::~VSPriority()
{
	m_pPriorityThanList.Clear();
}

VSPriority& VSPriority::operator= (VSPriority& rPriority)
{
	rPriority.CheckPriorityNum();
	m_uiPriorityNum = rPriority.m_uiPriorityNum;
	m_pPriorityThanList.Clear();
	m_pPriorityThanList = rPriority.m_pPriorityThanList;
	return *this;
}

bool VSPriority::operator== (VSPriority& rPriority)
{
	rPriority.CheckPriorityNum();
	CheckPriorityNum();
	return (m_uiPriorityNum == rPriority.m_uiPriorityNum);
}

bool VSPriority::operator!= (VSPriority& rPriority)
{
	rPriority.CheckPriorityNum();
	CheckPriorityNum();
	return (m_uiPriorityNum != rPriority.m_uiPriorityNum);
}

bool VSPriority::operator >(VSPriority& rPriority)
{
	rPriority.CheckPriorityNum();
	CheckPriorityNum();
	return (m_uiPriorityNum > rPriority.m_uiPriorityNum);
}
bool VSPriority::operator <(VSPriority& rPriority)
{
	rPriority.CheckPriorityNum();
	CheckPriorityNum();
	return (m_uiPriorityNum < rPriority.m_uiPriorityNum);
}

bool VSPriority::operator >=(VSPriority& rPriority)
{	
	rPriority.CheckPriorityNum();
	CheckPriorityNum();
	return (m_uiPriorityNum >= rPriority.m_uiPriorityNum);
}

bool VSPriority::operator <=(VSPriority& rPriority)
{
	rPriority.CheckPriorityNum();
	CheckPriorityNum();
	return (m_uiPriorityNum <= rPriority.m_uiPriorityNum);

}

uint32 VSPriority::CheckPriorityNum()
{
	if(m_uiPriorityNum || !m_pPriorityThanList.GetNum())
		return m_uiPriorityNum;
	else
	{
		uint32 uiMax = 0;
		for(uint32 i = 0; i < m_pPriorityThanList.GetNum() ; i++)
		{
			if(uiMax < m_pPriorityThanList[i]->CheckPriorityNum())
				uiMax = m_pPriorityThanList[i]->m_uiPriorityNum;
		
		}

		m_uiPriorityNum = uiMax + 1;
		return m_uiPriorityNum;
	}
}

bool VSPriority::CheckIsCircle(const VSPriority *pChecked)const
{
	if(!pChecked)
		return false;
	
	for(uint32 i = 0 ; i < m_pPriorityThanList.GetNum() ; i++)
	{
		if(pChecked == m_pPriorityThanList[i])
			return true;
		else
			return m_pPriorityThanList[i]->CheckIsCircle(pChecked);
	}

	return false;
}

}