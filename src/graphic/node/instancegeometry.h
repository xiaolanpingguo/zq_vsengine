#pragma once
#include "graphic/core/object.h"
#include "graphic/node/geometry.h"
namespace zq
{
	struct INSTANCE_DATA_TYPE
	{
		INSTANCE_DATA_TYPE(const VSTransform & t ,uint32 uiMaterialParaNum = 0)
		{
			T = t;
			MaterialData.SetBufferNum(uiMaterialParaNum);
		}
		VSTransform T;
		VSArray<VSArray<float>> MaterialData;
	};
	class GRAPHIC_API VSInstanceGeometry : public VSGeometry
	{
		//PRIORITY
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	protected:
		
		uint32 m_uiMaxInstanceNum;
		VSVertexFormatPtr m_pVertexFormat;
		uint32 m_uiInstanceSize;	
		unsigned char * GetCurDataBufferAddr(uint32 i);

		bool bHasUpdate = false;

		VSVertexBufferPtr m_InstanceBuffer;
		unsigned char * m_pInstanceDataBuffer;
		uint32 m_uiInstanceNum;
		//use for Multhread render 
		bool m_bReCreate;
		VSVertexBufferPtr m_PendingDeleteInstanceBuffer;
		unsigned char * m_pInstanceDataRenderBuffer;

		
	public:
		VSInstanceGeometry();
		
		virtual ~VSInstanceGeometry();
		void SetMaxInstanceBuffer(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &FormatArray, uint32 uiMaxNum);
		inline  VSVertexBuffer* GetInstanceVertexBuffer()const
		{
			return m_InstanceBuffer;
		}
		inline  VSVertexFormat* GetInstanceVertexFormat()const
		{
			return m_pVertexFormat;
		}
		virtual void UpdateGeometryBeforeDrawCall();
	public:
		void CreateInstanceBuffer(VSMeshData * pSourceMeshData, VSMaterialR * pMaterial);
		void ClearInstanceData();
		uint32 GetInstanceNum();
		uint32 AddInstance(const AABB WorldBound, const VSTransform & T, VSMaterialInstance * pMaterialInstance,VSArray<ANIM_INSTANCE_SHADER_DATA> * pAnimData= NULL);
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		inline bool IsUsed()const
		{
			return m_uiInstanceNum > 0;
		}
		void UpdateWorldInstanceBound();
		bool HaveSkinInstance();
		VSTexAllStatePtr m_AnimInstanceTexture;
		uint32 m_uiBoneNum = 0;
	};
	DECLARE_Ptr(VSInstanceGeometry);
	VSTYPE_MARCO(VSInstanceGeometry);


}
