#include "core/timer.h"
#include <MMSystem.h>

#pragma comment(lib, "winmm.lib")

namespace zq{


Timer * Timer::ms_pTimer = NULL;
Timer::Timer()
{
	InitGameTime();
	ms_pTimer = this;
}

Timer::~Timer()
{

}

void Timer::InitGameTime()
{
	m_iFrameCount = 0;
	m_fFPS = 0;
	m_fTime = 0;
	m_fLastFPSTime = 0;
	m_fTimeSlice = 0;
	m_fLastTime = 0;
	m_fDetTime = 0;
    if(QueryPerformanceFrequency((LARGE_INTEGER*) &m_int64OneSecondTicks))
    {
        m_bUseLargeTime=true;
        QueryPerformanceCounter((LARGE_INTEGER*) &m_int64TimeTickStartCounts);
    }
    else
    {
        m_bUseLargeTime = false;
        m_ulTimeStart = timeGetTime();
    }
}

int32 Timer::GetRandSeed()
{
	return ((LARGE_INTEGER*)&m_int64TimeTickStartCounts)->LowPart;
}

double Timer::GetGamePlayTime()
{  //返回已进行的时间,单位毫秒
	__int64 int64TimeCurrentCounts;
	if(m_bUseLargeTime)
	{
		QueryPerformanceCounter((LARGE_INTEGER*) &int64TimeCurrentCounts);
		return ((int64TimeCurrentCounts - m_int64TimeTickStartCounts)*(1.0 / m_int64OneSecondTicks)*1000.0);
	}
	else
	{
		return ((timeGetTime() - m_ulTimeStart));  //timeGetTime函数返回的时间单位为毫秒
	}
}

void Timer::UpdateFPS()
{

	m_fTime = GetGamePlayTime() * 0.001;
	m_fDetTime = m_fTime - m_fLastTime;
	m_fLastTime = m_fTime;
	if (m_fTime - m_fLastFPSTime > 1.0f)
	{
		m_fLastFPSTime = m_fTime;
		m_fFPS = m_iFrameCount;
		m_iFrameCount = 0;
	}
	else
	{
		m_iFrameCount++;
	}
}

}