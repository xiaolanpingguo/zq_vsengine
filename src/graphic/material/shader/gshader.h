#pragma once
#include "graphic/material/shader/shader.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSGShader : public VSShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSGShader();
		VSGShader(const char * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSGShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		virtual ~VSGShader();
		virtual uint32 GetShaderType()const
		{
			return VSEngineFlag::ST_GEOMETRY;
		}
	public:
		static const VSGShader *GetDefault()
		{
			return ms_Default;
		}
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		static VSPointer<VSGShader> ms_Default;
	};
	DECLARE_Ptr(VSGShader);
	VSTYPE_MARCO(VSGShader);
}
