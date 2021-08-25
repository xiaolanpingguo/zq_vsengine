#pragma once
#include "graphic/render/scenemanager/spatial.h"
//#include "datastruct/VSArray.h"
#include "graphic/render/buffer/vertexbuffer.h"
namespace zq
{

	class VSSkeleton;
	class VSMeshNode;
	class VSStream;
	DECLARE_Ptr(VSBoneNode);
	DECLARE_Ptr(VSMorphSet);
	DECLARE_Ptr(VSMeshData);
	DECLARE_Ptr(VSTexAllState);
	struct MorphDataType
	{
		uint32 Layer = 0;
		uint32 TextureSizeY = 0;
		VSMap<unsigned int, float> ValueMap;
		VSTexAllStatePtr MorphVDataTexture;
		VSTexAllStatePtr MorphNDataTexture;
		VSTexAllStatePtr MorphTDataTexture;
		VSTexAllStatePtr MorphBDataTexture;
	};
	class GRAPHIC_API VSGeometry : public VSSpatial
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum	// Update Geometry Renturn Information
		{
			 UGRI_FAIL,
			 UGRI_CONTINUME,
			 UGRI_END,			 
			 UGRI_MAX
		};
		enum // Geometry Use Type
		{
			GUT_NORMAL,
			GUT_SHADOW_VOLUME,
			GUT_MAX
		};
		VSGeometry();
		virtual ~VSGeometry();
		
		void SetMeshData(VSMeshData * pMeshData);	
		VSMeshData * GetMeshData()const;
		VSMeshData * GetOriginMeshData()const;

		uint32 GetActiveNum();
		bool SetActiveNum(uint32 uiActiveNum);
		uint32 GetTotalNum()const;
		uint32 GetMeshDataType();



		virtual void CreateLocalAABB();

		void SetAffectBoneArray(const VSArray<VSBoneNode *> & pBoneNodeArray);
		void SetAffectBoneArray(const VSArray<VSUsedName> & BoneNodeArray);
		inline uint32 GetAffectBoneNum()const
		{
			return m_pBoneNode.GetNum();
		}
		inline VSArray<Vector3W>& GetSkinWeightBuffer()
		{
			return m_SkinWeightBuffer;
		}
		inline VSBoneNode * GetAffectBone(uint32 i)const
		{
			if(i < m_pBoneNode.GetNum())
				return m_pBoneNode[i];
			else
				return NULL;
		}
		VSSkeleton * GetAffectSkeleton()const;

		VSMeshNode * GetMeshNode()const;
		uint32 GetVertexNum()const;


		//must send to render thread
		virtual void UpdateGeometryBeforeDrawCall();


		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
		inline void SetLocalBV(const AABB & BV)
		{
			m_LocalBV = BV;
		}
		inline AABB GetLocalBV()const
		{
			return m_LocalBV;
		}
		VSUsedName m_GeometryName;
		virtual bool IsSwapCull();
		virtual uint32 GetGeometryUseType()
		{
			return GUT_NORMAL;
		}
		static void LoadDefault();
		static VSGeometry * GetDefaultQuad()
		{
			return ms_Quad;
		}
		static VSGeometry * GetDefaultCube()
		{
			return ms_DefaultCube;
		}
		static VSGeometry * GetDefaultCubCone()
		{
			return ms_DefaultCubCone;
		}
		static VSGeometry * GetDefaultCone()
		{
			return ms_DefaultCone;
		}
		static VSGeometry *GetDefaultRenderCube()
		{
			return ms_DefaultRenderCube;
		}

		bool SetMaterialInstance(VSMaterialInstance * pMaterial, uint32 uiIndex);
		bool SetMaterialInstance(VSMaterialR * pMaterial, uint32 uiIndex);

		bool SetUseMaterialInstance(uint32 uiIndex);
		uint32 AddMaterialInstance(VSMaterialR * pMaterial);
		uint32 AddMaterialInstance(VSMaterialInstance * pMaterial);


		void ClearAllMaterialInstance();
		void DeleteMaterialInstance(uint32 i);
		uint32 DeleteMaterialInstance(VSMaterialInstance * pMaterial);

		VSMaterialInstance * GetUseMaterialInstance()const;

		VSMaterialInstance * GetMaterialInstance(uint32 i)const;
		void AddMorphAABB(VSVertexBuffer * pMorphVertexBuffer);
		void CreateMorphMeshData(VSMap<unsigned int, VSVertexBuffer *> & MorphDataSet);
		bool HasMorphTarget()const;
		const MorphDataType & GetMorphData()
		{
			return m_MorphData;
		}
		void SetMorphData(uint32 Index, float fData);
		virtual void GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const;
	protected:
		//friend class VSRenderer;
		VSArray<VSMaterialInstancePtr> m_pMaterialInstance;
		uint32 m_uiCurUseMaterial;

		virtual void UpdateView(VSCuller & Culler, double dAppTime);
		virtual void UpdateWorldBound(double dAppTime);//更新世界边界体 
		virtual void UpdateNodeAll(double dAppTime);
		virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
		virtual void UpdateOther(double dAppTime);
		VSMeshDataPtr m_pMeshData;
		uint32 m_uiActiveNum;
		VSArray<VSBoneNode *> m_pBoneNode;

		VSArray<VSUsedName> m_BoneName;
		AABB	m_LocalBV;
		void LinkBoneNode();
		VSArray<Vector3W> m_SkinWeightBuffer;
		MorphDataType m_MorphData;
		static VSPointer<VSGeometry> ms_Quad;
		static VSPointer<VSGeometry> ms_DefaultCube;
		static VSPointer<VSGeometry> ms_DefaultCubCone;
		static VSPointer<VSGeometry> ms_DefaultCone;
		static VSPointer<VSGeometry> ms_DefaultRenderCube;
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	};
	DECLARE_Ptr(VSGeometry);
	VSTYPE_MARCO(VSGeometry);

	class GRAPHIC_API VSShadowVolumeGeometry : public VSGeometry
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSShadowVolumeGeometry();
		virtual ~VSShadowVolumeGeometry();
		virtual uint32 GetGeometryUseType()
		{
			return GUT_SHADOW_VOLUME;
		}
		virtual void CreateClodMesh(){};
		virtual void RemoveClodMesh(){};
	};
	DECLARE_Ptr(VSShadowVolumeGeometry);
	VSTYPE_MARCO(VSShadowVolumeGeometry);
};
