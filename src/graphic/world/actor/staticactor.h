#pragma once
#include "graphic/world/actor/actor.h"
#include "graphic/node/nodecomponent/staticmeshcomponent.h"
namespace zq
{
	DECLARE_Proxy(VSStaticMeshNode);
	class GRAPHIC_API VSStaticActor : public VSActor
	{
		//PRIORITY
	
		//RTTI
		DECLARE_RTTI;
	public:
		friend class VSWorld;
		VSStaticActor();
		virtual ~VSStaticActor();
		DECLARE_INITIAL


		GET_TYPE_NODE(VSStaticMeshComponent)
		virtual void CreateDefaultComponentNode();

	};
	DECLARE_Ptr(VSStaticActor);
	VSTYPE_MARCO(VSStaticActor);
}
