#pragma once
#include "graphic/material/shader/shader.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSDShader : public VSShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSDShader();
		VSDShader(const char * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSDShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		virtual ~VSDShader();
		virtual uint32 GetShaderType()const
		{
			return VSEngineFlag::ST_DOMAIN;
		}
	public:
		static const VSDShader *GetDefault()
		{
			return ms_Default;
		}
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		static VSPointer<VSDShader> ms_Default;
	};
	DECLARE_Ptr(VSDShader);
	VSTYPE_MARCO(VSDShader);
}
