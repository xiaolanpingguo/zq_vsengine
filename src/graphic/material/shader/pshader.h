#pragma once
#include "graphic/material/shader/shader.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSPShader : public VSShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSPShader();
		VSPShader(const char * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSPShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		virtual ~VSPShader();
		virtual uint32 GetShaderType()const
		{
			return VSEngineFlag::ST_PIXEL;
		}
	public:
		static const VSPShader *GetDefault()
		{
			return ms_Default;
		}
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		static VSPointer<VSPShader> ms_Default;
	};
	DECLARE_Ptr(VSPShader);
	VSTYPE_MARCO(VSPShader);

}
