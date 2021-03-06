#pragma once
#include "graphic/node/model/modelmeshnode.h"
namespace zq
{
	DECLARE_Proxy(VSStaticMeshNode);
	class GRAPHIC_API VSStaticMeshNode : public VSModelMeshNode, public VSResource
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSStaticMeshNode();
		virtual ~VSStaticMeshNode();
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		void AddLodMesh(VSStaticMeshNodeR * pStaticMeshResource);

		DECLARE_RESOURCE(VSStaticMeshNode, RA_ASYN_LOAD | RA_ENABLE_GC, RT_STATIC_MODEL,STMODEL, Resource/StaticMesh, VSCacheResource)

	};
	DECLARE_Ptr(VSStaticMeshNode);
	VSTYPE_MARCO(VSStaticMeshNode);
	DECLARE_Proxy(VSStaticMeshNode);
}
