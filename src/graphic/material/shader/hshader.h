#pragma once
#include "graphic/material/shader/shader.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSHShader : public VSShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSHShader();
		VSHShader(const char * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSHShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		virtual ~VSHShader();
		virtual uint32 GetShaderType()const
		{
			return VSEngineFlag::ST_HULL;
		}
	public:
		static const VSHShader *GetDefault()
		{
			return ms_Default;
		}
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		static VSPointer<VSHShader> ms_Default;
	};
	DECLARE_Ptr(VSHShader);
	VSTYPE_MARCO(VSHShader);
}
