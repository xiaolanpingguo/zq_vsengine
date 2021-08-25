#pragma once


#include "graphic/core/graphic.h"
#include "datastruct/VSArray.h"


namespace zq{


class GRAPHIC_API VSSlot 
{
public:
	VSSlot();
	~VSSlot();
		
	VSArray<unsigned char> m_VSTexSlot;
	VSArray<unsigned char> m_PSTexSlot;
	VSArray<unsigned char> m_GSTexSlot;
	VSArray<unsigned char> m_DSTexSlot;
	VSArray<unsigned char> m_HSTexSlot;
	VSArray<unsigned char> m_CSTexSlot;
	void ClearAllSlot();
	bool HasAnySlot();
	bool m_bBindResourceUse;
	inline uint32 GetOutputType()const
	{
		return m_uiOutputType;
	}
protected:
	uint32 m_uiOutputType;
};


}