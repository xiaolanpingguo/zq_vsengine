#pragma once


#include "graphic/core/object.h"
#include "graphic/controller/animtree/timeevent.h"


namespace zq{


class GRAPHIC_API VSStartAnimEvent : public VSTimeEvent
{
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	VSStartAnimEvent();
	~VSStartAnimEvent();
	VSStartAnimEvent(VSController *pController);
		
};
DECLARE_Ptr(VSStartAnimEvent);
VSTYPE_MARCO(VSStartAnimEvent);


class GRAPHIC_API VSEndAnimEvent : public VSTimeEvent
{
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	VSEndAnimEvent();
	~VSEndAnimEvent();
	VSEndAnimEvent(VSController *pController);

};
DECLARE_Ptr(VSEndAnimEvent);
VSTYPE_MARCO(VSEndAnimEvent);


class GRAPHIC_API VSController : public VSObject
{
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL_NO_CLASS_FACTORY
public:
    friend class VSObject;
    VSController();
	virtual ~VSController() = 0;


    inline VSObject* GetObject()const
    {
        return m_pObject;
    }

    inline void ClearTime()
    {
        m_bStart = false;
    }

	virtual bool Update(double dAppTime);
	virtual bool UpdateEx(double aAppTime);

	enum // Repeat Type
	{
		RT_NONE,
		RT_CLAMP,
		RT_WRAP,
		RT_CYCLE,
		RT_REVEERSE_CLAMP,
		RT_REVEERSE_WRAP,
		RT_REVEERSE_CYCLE,
		RT_MAX
	};
	uint32 m_uiRepeatType;

	double		m_dMinTime;
	double		m_dMaxTime;
	double		m_dPhase;
	double		m_dFrequency;
		
	static double GetTime(double fTime, double fMin,double fMax,uint32 uiRepeatType);
public:

	virtual bool SetObject(VSObject * pObject);
	double	GetControlTime(double dAppTime);

	VSObject* m_pObject;
	double	m_dNowAnimTime;
	double	m_dLastAppTime;
	double	m_dLastAnimTime;
	double  m_dIntervalTime;
	double  m_dIntervalAnimTime;
	double	m_dTimeSum;
	double	m_dStartSystemTime;
	bool	m_bStart;

	void AddTimeEvent(VSTimeEvent * pTimeEvent);
	void DeleteTimeEvent(VSTimeEvent * pTimeEvent);
	void SetEnable(bool Enable);

	void AddTriggerStart(TriggerAnimEventType::Handler handler);

	void AddTriggerStop(TriggerAnimEventType::Handler handler);

	void AddTriggerEnd(TriggerAnimEventType::Handler handler);

	void AddTriggerBeginStart(TriggerAnimEventType::Handler handler);
public:
		
	bool m_bIsVisibleUpdate;
protected:
	VSArray<VSTimeEventPtr> m_TimeEventArray;
	void TimeEvent(double dAppTime);
	float m_fInnerTimeScale;
	bool m_bEnable;
	TriggerAnimEventType m_TriggerBeginStart;
	TriggerAnimEventType m_TriggerStop;
};


DECLARE_Ptr(VSController);
VSTYPE_MARCO(VSController);
}
