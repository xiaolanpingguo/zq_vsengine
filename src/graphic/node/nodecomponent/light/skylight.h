#pragma once
#include "graphic/node/nodecomponent/light/light.h"
namespace zq
{
	
	class VSStream;
	class GRAPHIC_API VSSkyLight : public VSIndirectLight
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSSkyLight();
		virtual ~VSSkyLight();
		VSColorRGBA m_UpColor;
		VSColorRGBA m_DownColor;
		virtual uint32 GetLightType()const
		{
			return LT_SKY;
		}
	};
	DECLARE_Ptr(VSSkyLight);
	VSTYPE_MARCO(VSSkyLight);
}
