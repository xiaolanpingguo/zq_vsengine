#pragma once


#include "core/system.h"


namespace zq{


class SYSTEM_API Timer
{
private:
	bool m_bUseLargeTime;   //ʹ�ô�ʱ���־
		__int64 m_int64OneSecondTicks;  //һ���ڵĵδ����
		__int64 m_int64TimeTickStartCounts;  //��ʼ�ĵδ����ֵ
	unsigned long m_ulTimeStart;  //timeGetTime��ʼʱ��
	int32 m_iFrameCount;
	double m_fFPS;
	double m_fTime,m_fLastFPSTime,m_fTimeSlice;
	double m_fDetTime, m_fLastTime;
	void InitGameTime();
public:
	Timer();
	~Timer();
	
	double GetGamePlayTime();
	void UpdateFPS();
	inline double GetFPS(){return m_fFPS;}
	static Timer * ms_pTimer;
	double GetDetTime(){ return m_fDetTime; }
	int32 GetRandSeed();
};
}
