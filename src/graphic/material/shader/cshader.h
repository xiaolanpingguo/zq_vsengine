#pragma once
#include "graphic/material/shader/shader.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSCShader : public VSShader
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSCShader();
		VSCShader(const char * pBuffer, const VSString & MainFunName, bool IsFromFile = false);
		VSCShader(const VSString &Buffer, const VSString & MainFunName, bool IsFromFile = false);
		virtual ~VSCShader();
		virtual uint32 GetShaderType()const
		{
			return VSEngineFlag::ST_COMPUTE;
		}
	public:
		static const VSCShader *GetDefault()
		{
			return ms_Default;
		}
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);
		static VSPointer<VSCShader> ms_Default;
	};
	DECLARE_Ptr(VSCShader);
	VSTYPE_MARCO(VSCShader);
}
