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
	//0�ȱ�ʾ���ȼ������Ҳ����û�����ȼ������m_pPriorityThanList�޳�Ա����ʾ������޼���
	//������Ҫ�������ȼ���
	uint32 m_uiPriorityNum;
	VSArray<VSPriority *> m_pPriorityThanList;
};


}
