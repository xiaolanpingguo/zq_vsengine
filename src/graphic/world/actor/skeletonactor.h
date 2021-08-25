#pragma once
#include "graphic/world/actor/actor.h"
#include "graphic/node/nodecomponent/skeletonmeshcomponent.h"
namespace zq
{
	class GRAPHIC_API VSSkeletonActor : public VSActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		friend class VSWorld;
		VSSkeletonActor();
		virtual ~VSSkeletonActor();

		DECLARE_INITIAL


		GET_TYPE_NODE(VSSkeletonMeshComponent)
		//virtual void Update(double dAppTime);
		virtual void CreateDefaultComponentNode();
	};
	DECLARE_Ptr(VSSkeletonActor);
	VSTYPE_MARCO(VSSkeletonActor);
}
