#pragma once
#include "graphic/node/meshnode.h"
#include "graphic/node/geometrynode.h"
namespace zq
{
	class VSGeometryNode;
	class VSSwitchNode;
	class VSStream;
	class GRAPHIC_API VSModelMeshNode : public VSMeshNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:

		VSModelMeshNode();
		virtual ~VSModelMeshNode();

		virtual VSSwitchNode * GetLodNode()const;
		virtual VSGeometryNode * GetGeometryNode(uint32 uiLodLevel);
		void SetUseMaterialInstance(uint32 SubMeshID,uint32 UsedID);
		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual void UpdateController(double dAppTime);
		void SetMorphData(const VSUsedName & ShowName, float fData);
		virtual bool SetDrawInstance(bool bDrawInstance);
		void ClearLodMesh();
		virtual void GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const;
	protected:
		bool AddLODGeometryNode(VSGeometryNode* pGeoNode);

	};
	DECLARE_Ptr(VSModelMeshNode);
	VSTYPE_MARCO(VSModelMeshNode);
}
