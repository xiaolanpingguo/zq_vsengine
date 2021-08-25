#pragma once
#include "graphic/core/name.h"
#include "graphic/node/nodecomponent/light/light.h"
namespace zq
{
	class VSGeometry;
	class VSVertexBuffer;
	class VSStream;
	DECLARE_Ptr(VSGeometry);
	DECLARE_Ptr(VSVertexBuffer);
	DECLARE_Ptr(VSMaterialInstance);
	DECLARE_Ptr(VSLight);

	struct MaterialShaderPara 
	{
		MaterialShaderPara()
		{
			pGeometry = NULL;
			pShadowLight = NULL;
			pCamera = NULL;
			pMaterialInstance = NULL;
			LightArray.Clear();
			uiPassId = 0;
			uiCompileShaderType = VSEngineFlag::ST_MAX;
			bUseTessellation = false;
		}
		~MaterialShaderPara()
		{
			pGeometry = NULL;
			pShadowLight = NULL;
			pCamera = NULL;
			pMaterialInstance = NULL;
			LightArray.Clear();
			uiPassId = 0;

		}
		VSCamera* pCamera;
		VSMaterialInstance* pMaterialInstance;
		VSArray<VSLight*>	LightArray;
		uint32 uiPassId;
		uint32 uiPassType;
		VSGeometry* pGeometry;
		VSLight * pShadowLight;
		VSString VSShaderPath;
		VSString PSShaderPath;
		VSString GSShaderPath;
		VSString HSShaderPath;
		VSString DSShaderPath;

		VSString VMainFunName;
		VSString PMainFunName;
		VSString GMainFunName;
		VSString HMainFunName;
		VSString DMainFunName;
		VSColorRGBA SkyLightUpColor;
		VSColorRGBA SkyLightDownColor;
		uint32 uiCompileShaderType;
		bool bUseTessellation;
	};
	class VSStream;
	class GRAPHIC_API VSShaderKey : public VSObject
	{
		
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSShaderKey();
		~VSShaderKey();
	protected:
		VSMap<VSUsedName,unsigned int> m_KeyMap;
	public:
		enum
		{

			MAX_LIGTH_TYPE_MASK = 3,
			MAX_LIGHT_TYPE_NUM = (1 << MAX_LIGTH_TYPE_MASK) - 1
		};
		bool IsHaveTheKey(const VSUsedName & Name,uint32 &uiKeyId);
		void SetTheKey(const VSUsedName & Name,uint32 Value);

	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();

		static void SetMaterialVShaderKey(VSShaderKey * pKey,MaterialShaderPara & MSPara);

		static void SetMaterialPShaderKey(VSShaderKey * pKey,MaterialShaderPara & MSPara);

		static void SetMaterialGShaderKey(VSShaderKey * pKey, MaterialShaderPara & MSPara);

		static void SetMaterialHShaderKey(VSShaderKey * pKey, MaterialShaderPara & MSPara);

		static void SetMaterialDShaderKey(VSShaderKey * pKey, MaterialShaderPara & MSPara);
		struct ShadowKeyInfo
		{
			ShadowKeyInfo()
			{
				uiShadowNum = 0;
				uiVolumeShadowNum = 0;
				uiDualParaboloidShadowNum = 0 ;
				uiCSMShadowNum = 0;
			}
			uint32 uiShadowNum;
			uint32 uiVolumeShadowNum;
			uint32 uiDualParaboloidShadowNum;
			uint32 uiCSMShadowNum;
		};
		static uint32 GenerateKey(const VSArray<VSLight*> & LightArray,ShadowKeyInfo ShadowInfo[VSLight::LT_MAX],uint32 &uiLightFunctionKey);

		static uint32 GetLightNum(uint32 uiLightKey,uint32 uiLightType);
		
		GRAPHIC_API friend bool operator == (const VSShaderKey & Key1,const VSShaderKey & Key2);
		GRAPHIC_API friend bool operator > (const VSShaderKey & Key1,const VSShaderKey & Key2);
		GRAPHIC_API friend bool operator < (const VSShaderKey & Key1,const VSShaderKey & Key2);

		//ÖØÔØ=²Ù×÷·û
		void operator =(const VSShaderKey &ShaderKey);

		void GetDefine(VSMap<VSString,VSString> & Define);

		void Clear();
		static VSPointer<VSShaderKey> ms_NULLShaderKey;
	};
	VSTYPE_MARCO(VSShaderKey);
}
