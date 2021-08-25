#pragma once
#include "graphic/core/object.h"
#include "graphic/material/shader/userconstant.h"
#include "graphic/controller/controller.h"
#include "graphic/material/shader/usersampler.h"
#include "graphic/material/shader/userbuffer.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/render/bind.h"
#include "graphic/core/resource.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSShader : public VSBind
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		VSShader(const char * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		VSShader();
		virtual ~VSShader() = 0;
		virtual uint32 GetShaderType()const = 0;
	public:
		virtual bool SetParam(const VSUsedName &Name,void * pData);
		virtual bool SetParam(const VSUsedName &Name,VSTexAllState * pTexture,uint32 uiIndex = 0);
		virtual bool SetParam(const VSUsedName &Name, VSBufferResource * pBufferResource, uint32 uiIndex = 0);
		inline void ClearBuffer()
		{
			m_Buffer.Clear();
		}
		bool HasBindResource();
		
		inline const VSString & GetBuffer()const{ return m_Buffer;}
		inline const void *GetCacheBuffer()const
		{
			return m_pCacheBuffer;
		}
		inline uint32 GetCacheBufferSize()const
		{
			return m_uiCacheBufferSize;
		}
		bool SetCacheBuffer(void * pBuffer, uint32 uiSize);
		const VSString &GetMainFunName()const
		{
			return m_MainFunName;
		}
		uint32 m_uiConstBufferSize;

		inline const VSUsedName & GetFileName()const
		{
			return m_FileName;
		}

		inline void SetFileName(const VSUsedName & FileName)
		{
			m_FileName = FileName;
		}
	protected:
		VSString m_Buffer;
		unsigned char * m_pCacheBuffer;
		uint32 m_uiCacheBufferSize;
		VSString	m_MainFunName;
		VSUsedName	m_FileName;
	public:	
		void SetShaderString(const char * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		void SetShaderString(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		inline uint32 GetMinShareSampler()const
		{
			return m_uiShareSamplerRegister.GetNum() > 0 ? m_uiShareSamplerRegister[0].Key : INVALID_SIMPLAR_REGISTER;
		}
		inline uint32 GetMaxShareSampler()const
		{
			return m_uiShareSamplerRegister.GetNum() > 0 ? m_uiShareSamplerRegister[m_uiShareSamplerRegister.GetNum() - 1].Key : INVALID_SIMPLAR_REGISTER;
		}
		void AddShareSampler(uint32 ShareSamplerRegister, uint32 ShareSamplerType);
	public:
		VSArray<VSUserConstantPtr>		m_pUserConstant;
		VSArray<VSUserSamplerPtr>		m_pUserSampler;
		VSArray<VSUserBufferPtr>		m_pUserBuffer;
		VSShaderKey						m_ShaderKey;
		bool		m_bCreatePara;
		uint32 m_uiArithmeticInstructionSlots;
		uint32 m_uiTextureInstructionSlots;
		VSMap<unsigned int, unsigned int> m_uiShareSamplerRegister;
	};
	DECLARE_Ptr(VSShader);
	VSTYPE_MARCO(VSShader);

}
