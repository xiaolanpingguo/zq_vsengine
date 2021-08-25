#pragma once


#include "datastruct/VSArray.h"
#include "graphic/core/graphic.h"
#include "graphic/core/prioritymarc.h"
#include "stl/vector.h"


namespace zq{


class GRAPHIC_API VSPriority
{
public:
	VSPriority();
	VSPriority(uint32 uiPriorityNum);
	~VSPriority();
	bool AddPriorityThan(VSPriority * pPriority);
	uint32 GetPriorityNum();

	VSPriority& operator= (VSPriority& rPriority);
	bool operator== (VSPriority& rPriority);
	bool operator!= (VSPriority& rPriority);	
	bool operator >(VSPriority& rPriority);
	bool operator <(VSPriority& rPriority);	
	bool operator >=(VSPriority& rPriority);
	bool operator <=(VSPriority& rPriority);

private:
	bool CheckIsCircle(const VSPriority *pChecked)const;
	uint32 CheckPriorityNum();
	//0既表示优先级别最大，也或者没有优先级。如果m_pPriorityThanList无成员，表示最大有限级别
	//否则需要计算优先级别
	uint32 m_uiPriorityNum;
	VSArray<VSPriority *> m_pPriorityThanList;
};


}
