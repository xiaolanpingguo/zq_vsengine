#pragma once
#include "graphic/core/object.h"
#include "graphic/render/bind.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSVertexFormat : public VSBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:

		VSVertexFormat();
		virtual ~VSVertexFormat();
		
		enum
		{
			VF_POSITION,
			VF_TEXCOORD,
			VF_NORMAL,
			VF_TANGENT,
			VF_BINORMAL,
			VF_PSIZE,
			VF_COLOR,
			VF_FOG,
			VF_VERTEX_ID,
			VF_BLENDWEIGHT,
			VF_BLENDINDICES,
			VF_MATERIAL_INSTANCE,
			VF_MAX
		};
		struct VERTEXFORMAT_TYPE
		{
		public:
			enum
			{
				IDSR_PER_VERTEX_DATA,
				IDSR_PER_INSTANCE_DATA,
			};
			VERTEXFORMAT_TYPE()
			{
				Stream = 0;
				Offset = 0;
				DataType = 0;
				Semantics = 0;
				SemanticsIndex = 0;
				InstanceDataStepRate = IDSR_PER_VERTEX_DATA;
			}
			~VERTEXFORMAT_TYPE()
			{

			}
			UINT Stream;
			UINT Offset;
			UINT DataType;
			UINT Semantics;
			UINT SemanticsIndex;
			UINT InstanceDataStepRate;
		};
	public:
		friend class VSRenderer;
		friend class VSDX9Renderer;

		friend class VSVShaderKey;
		friend class VSPShaderKey;
		friend class VSResourceManager;
		friend class VSShaderKey;

	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		uint32 m_uiVertexFormatCode;
	public:
		
		VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> m_FormatArray;
		inline uint32 GetVertexFormatCode()const
		{
			return m_uiVertexFormatCode;
		}

	};
	DECLARE_Ptr(VSVertexFormat);
	VSTYPE_MARCO(VSVertexFormat);

}
