#pragma once


#include "core/system.h"


namespace zq{


class SYSTEM_API Timer
{
private:
	bool m_bUseLargeTime;   //使用大时间标志
		__int64 m_int64OneSecondTicks;  //一秒内的滴答次数
		__int64 m_int64TimeTickStartCounts;  //开始的滴答计数值
	unsigned long m_ulTimeStart;  //timeGetTime开始时间
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
