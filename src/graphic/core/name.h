#pragma once


#include "graphic/core/type.h"
#include "graphic/core/reference.h"
#include "graphic/core/priority.h"
#include "graphic/core/stream/customarchiveobject.h"
#include "graphic/core/initialmarc.h"


namespace zq{


class GRAPHIC_API VSName : public VSReference,public MemObject
{
public:
	friend class VSResourceManager;
	~VSName();
	explicit VSName(const char* pChar,uint32 uiID);
	explicit VSName(const VSString & VSString,uint32 uiID);

	inline const char* GetBuffer()const
	{
		return m_String.GetBuffer();
	}

	inline const VSString& GetString()const
	{
		return m_String;
	}

	inline uint32 GetLength()const
	{
		return m_String.GetLength();
	}

	inline uint32 GetID()const
	{
		return m_uiID;
	}
		
protected:
		
	uint32 m_uiID;
    VSString m_String;
};

DECLARE_Ptr(VSName);

class VSStream;
class GRAPHIC_API VSUsedName : public VSCustomArchiveObject
{
	//PRIORITY
	DECLARE_PRIORITY
public:
	~VSUsedName();
	VSUsedName();
	/*explicit */VSUsedName(const char * pChar);
	/*explicit */VSUsedName(const VSString & VSString);

	//重载=操作符
	void operator =(const VSString &VSString);
	void operator =(const char *pChar);
	void operator =(const VSUsedName & Name);
	inline const char* GetBuffer()const
	{
		if (!m_pName)
		{
			return NULL;
		}

		return m_pName->GetBuffer();
	}

	inline const VSString& GetString()const
	{
		if (!m_pName)
		{
			return VSString::ms_StringNULL;
		}

		return m_pName->GetString();
	}

	inline uint32 GetLength()const
	{
		if (!m_pName)
		{
			return 0;
		}
		return m_pName->GetLength();
	}

	inline uint32 GetNameCode()const
	{
		if (!m_pName)
		{
			return 0;
		}
		return m_pName->GetID();
	}

	inline const VSString&  operator *() const
	{
		return GetString();
	}
	
	GRAPHIC_API friend bool operator >(const VSUsedName &Name1,const VSUsedName &Name2);
	
	GRAPHIC_API friend bool operator <(const VSUsedName &Name1,const VSUsedName &Name2);

	//重载==操作符
	GRAPHIC_API friend bool operator ==(const VSUsedName &Name1,const VSUsedName &Name2);

	//重载!=操作符
	GRAPHIC_API friend bool operator !=(const VSUsedName &Name1,const VSUsedName &Name2);

	//重载==操作符
	GRAPHIC_API friend bool operator ==(const VSUsedName &Name,const VSString & VSString);

	//重载!=操作符
	GRAPHIC_API friend bool operator !=(const VSUsedName &Name,const VSString & VSString);

	//重载==操作符
	GRAPHIC_API friend bool operator ==(const VSUsedName &Name,const char * pChar);

	//重载!=操作符
	GRAPHIC_API friend bool operator !=(const VSUsedName &Name,const char * pChar);

	virtual void Archive(VSStream & Stream);
	virtual void CopyFrom(VSCustomArchiveObject*, VSMap<VSObject*, VSObject*>& CloneMap);
protected:
	VSNamePtr m_pName;
	
	DECLARE_INITIAL_ONLY
	static bool InitialDefaultState();
	static bool TerminalDefaultState();
public:
	//Shader key
	static VSUsedName ms_cPassID;
	static VSUsedName ms_cPreZBeUsedBone;
	static VSUsedName ms_cUseInstance;
	static VSUsedName ms_cUseTessellation;
	static VSUsedName ms_cGPUMorphTarget;
	static VSUsedName ms_cMaterialVertexFormat;
	static VSUsedName ms_cVolumeVertexFormat;
	static VSUsedName ms_cLightFunKey;
	static VSUsedName ms_cMaterialLightKey;
	static VSUsedName ms_cLighted;
	static VSUsedName ms_cCubShadowFov;
	static VSUsedName ms_cPointLightShadowNum;
	static VSUsedName ms_cPointLightVolumeShadowNum;
	static VSUsedName ms_cPointLightParaboloidShadowNum;
	static VSUsedName ms_cDirectionLightShadowNum;
	static VSUsedName ms_cDirectionLightVolumeShadowNum;
	static VSUsedName ms_cDirectionLightCSMShadowNum;
	static VSUsedName ms_cSpotLightShadowNum;
	static VSUsedName ms_cBlinnPhong;
	static VSUsedName ms_cSimpleAnimInstanceLerp;
	static VSUsedName ms_cOrenNayarLookUpTable;

	//Inner Shader
	static VSUsedName ms_cVolumeShadowShader;
	static VSUsedName ms_cLightFunShader;
	static VSUsedName ms_cPostEffectShader;
	static VSUsedName ms_cPreZShader;
	static VSUsedName ms_cHullShader;

	//Custom Shader
	static VSUsedName ms_cPostGray;
	static VSUsedName ms_cGammaCorrect;
	static VSUsedName ms_cPostScreenQuad;
	static VSUsedName ms_cPostSaturation;
	static VSUsedName ms_cOldPhoto;
	static VSUsedName ms_cGuassBlurH;
	static VSUsedName ms_cGuassBlurV;
	static VSUsedName ms_cFilter3X3;
	static VSUsedName ms_cBright;
	static VSUsedName ms_cTexAdd;
	static VSUsedName ms_cPostVolumeShadowMap;

	//Shader Parameter
	static VSUsedName ms_cPostInputTexture[POSTEFFECT_MAX_INPUT];
	static VSUsedName ms_cPostInv_Width;
	static VSUsedName ms_cPostInv_Height;
	static VSUsedName ms_cTexOffsetAndWeight;
	static VSUsedName ms_cWorldViewProjectMatrix;
	static VSUsedName ms_cProjectShadowMatrix;
	static VSUsedName ms_cViewProjectMatrix;
	static VSUsedName ms_cWorldMatrix;

	//Only Name
	static VSUsedName ms_cMainScene;
	static VSUsedName ms_cPositionInfo;
	static VSUsedName ms_cClearColor;
	static VSUsedName ms_cDepth;

	static VSUsedName ms_cNULL;
};


CUSTOMTYPE_MARCO(VSUsedName)
	
}
