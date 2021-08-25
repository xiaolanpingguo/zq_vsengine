#pragma once


#include "graphic/core/object.h"
#include "datastruct/VSDelegate.h"


namespace zq{


typedef VSDelegateEvent<void(void)> TriggerAnimEventType;

class VSStream;
class VSController;

class GRAPHIC_API VSTimeEvent : public VSObject
{
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL_NO_CLASS_FACTORY
public:
	virtual ~VSTimeEvent() = 0;
	inline VSController * GetObject()const;

	VSTimeEvent();
	VSTimeEvent(VSController *pController);
	virtual void Trigger();
public:
	bool m_bEnable;
	float m_fTriggerPercent;
	TriggerAnimEventType m_TriggerAnimEvent;
protected:
	VSController * m_pController;
		
};

DECLARE_Ptr(VSTimeEvent);
VSTYPE_MARCO(VSTimeEvent);

}
