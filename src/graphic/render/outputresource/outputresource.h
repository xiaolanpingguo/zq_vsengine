#pragma once
#include "graphic/render/inheritbind.h"
#include "graphic/render/bindlockinterface.h"
namespace zq
{
	class GRAPHIC_API VSOutputResource : public VSInheritBind , public VSBindLockInterface
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		virtual void SetLockFlag(uint32 uiLockFlag) {};
		friend class VSTexture;
		friend class VSResourceManager;
		virtual ~VSOutputResource();
		bool m_bUsed;
		enum // Output Type
		{
			OT_NONE,
			OT_RENDER_TARGET,
			OT_DEPTH_STENCIL,
			OT_BUFFER_UNORDER_ACCESS,
			OT_TEXTURE_UNORDER_ACCESS,
			OT_MAX
		};
		virtual uint32 GetOutputType()const = 0;
		virtual VSBind * GetCreateBy()const
		{
			return m_pCreateBy;
		}	
	protected:
		VSOutputResource();
		VSBindPtr m_pCreateBy;
	};
	DECLARE_Ptr(VSOutputResource);
	VSTYPE_MARCO(VSOutputResource);
}

