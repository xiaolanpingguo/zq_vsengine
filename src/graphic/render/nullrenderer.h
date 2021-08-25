#pragma once
#include "graphic/render/renderer.h"
namespace zq
{
	class GRAPHIC_API VSNullRenderer : public VSRenderer
	{
	public:
		VSNullRenderer();
		virtual ~VSNullRenderer();
		virtual int32 GetRendererType() const{ return RAT_NULL; }
		virtual void ClearBuffers(uint32 uiClearFlag);
		virtual void ClearBackBuffer();
		virtual void ClearZBuffer();
		virtual void ClearStencilBuffer();
		virtual void ClearBuffers();

		virtual void ClearBuffers(uint32 uiClearFlag, int32 iXPos, int32 iYPos, int32 iWidth,
			int32 iHeight);
		virtual void ClearBackBuffer(int32 iXPos, int32 iYPos, int32 iWidth,
			int32 iHeight);
		virtual void ClearZBuffer(int32 iXPos, int32 iYPos, int32 iWidth,
			int32 iHeight);
		virtual void ClearStencilBuffer(int32 iXPos, int32 iYPos, int32 iWidth,
			int32 iHeight);
		virtual void ClearBuffers(int32 iXPos, int32 iYPos, int32 iWidth,
			int32 iHeight);

		virtual bool SetViewPort(VSViewPort *pViewPort = NULL);
		virtual bool UseWindow(int32 uiWindowID = -1);

		virtual bool CooperativeLevel();
		virtual bool BeginRendering();
		virtual bool EndRendering();

		virtual void DeviceLost();
		virtual void ResetDevice();
		virtual bool ChangeScreenSize(uint32 uiWidth, uint32 uiHeight, bool bWindow);
		virtual bool DrawMesh(VSGeometry * pGeometry, VSRenderState * pRenderState, VSVShader * pVShader, VSPShader * pPShader, 
			VSGShader * pGShader, VSHShader * pHShader, VSDShader * pDShader);
		virtual bool DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4]);
		virtual bool DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer, uint32 uiVertexNum,
			MATH_USHORT_INDEX * pIndexBuffer, uint32 uiIndexNum);
		virtual bool DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, uint32 uiVertexNum,
			MATH_USHORT_INDEX * pIndexBuffer, uint32 uiIndexNum);
		virtual uint32 SetBlendState(VSBlendState * pBlendState, bool bForceSet = false);
		virtual uint32 SetDepthStencilState(VSDepthStencilState * pDepthStencilState, bool bForceSet = false);
		virtual uint32 SetRasterizerState(VSRasterizerState * pRasterizerState, bool bForceSet = false);
		virtual void SetClipPlane(VSArray<Plane> &Plane, bool bEnable);
		virtual void SetScissorRect(VSArray<VSRect2> &Rect, bool bEnable);

		virtual uint32 SetVBuffer(VSVertexBuffer * pVBuffer,uint32 uiStream = 0);
		virtual uint32 SetIBuffer(VSIndexBuffer* pIBuffer);

		virtual uint32 BeginQuery(VSQuery * pQuery);
		virtual uint32 EndQuery(VSQuery * pQuery);

		virtual uint32 SetPTexture(VSTexture* pTexture, uint32 i);
		virtual uint32 SetVTexture(VSTexture* pTexture, uint32 i);
		virtual uint32 SetGTexture(VSTexture* pTexture, uint32 i);
		virtual uint32 SetDTexture(VSTexture* pTexture, uint32 i);
		virtual uint32 SetHTexture(VSTexture* pTexture, uint32 i);
		virtual uint32 SetCTexture(VSTexture* pTexture, uint32 i);
		virtual uint32 SetCBufferResource(VSBufferResource* pBufferResource, uint32 i);
		virtual uint32 SetVBufferResource(VSBufferResource* pBufferResource, uint32 i);
		virtual uint32 SetPBufferResource(VSBufferResource* pBufferResource, uint32 i);
		virtual uint32 SetGBufferResource(VSBufferResource* pBufferResource, uint32 i);
		virtual uint32 SetHBufferResource(VSBufferResource* pBufferResource, uint32 i);
		virtual uint32 SetDBufferResource(VSBufferResource* pBufferResource, uint32 i);
		virtual void Dispath(uint32 uiThreadGroupCountX, uint32 uiThreadGroupCountY,
			uint32 uiThreadGroupCountZ);

		virtual uint32 SetPSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet = false);
		virtual uint32 SetVSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet = false);
		virtual uint32 SetGSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet = false);
		virtual uint32 SetDSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet = false);
		virtual uint32 SetHSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet = false);
		virtual uint32 SetCSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet = false);
		
		virtual bool SetDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer);
		virtual bool EndDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer);
		virtual uint32 GetTextureCompressLen(uint32 uiLen);
		virtual const VSAdapterInfo* GetAdapterInfo(uint32 & uiAdapterNum);
	protected:

		virtual void SetVShaderConstant(VSVShader * pShader);
		virtual void SetVShaderBindResource(VSVShader * pShader);
		virtual void SetPShaderConstant(VSPShader * pShader);
		virtual void SetPShaderBindResource(VSPShader * pShader);
		virtual void SetGShaderConstant(VSGShader * pShader);
		virtual void SetGShaderBindResource(VSGShader * pShader);
		virtual void SetDShaderConstant(VSDShader * pShader);
		virtual void SetDShaderBindResource(VSDShader * pShader);
		virtual void SetHShaderConstant(VSHShader * pShader);
		virtual void SetHShaderBindResource(VSHShader * pShader);
		virtual void SetCShaderConstant(VSCShader * pShader);
		virtual void SetCShaderBindResource(VSCShader * pShader);


		virtual bool SetVShaderProgram(VSVShader*	pVShaderProgram);
		virtual bool SetPShaderProgram(VSPShader*	pPShaderProgram);
		virtual bool SetGShaderProgram(VSGShader*	pGShaderProgram);
		virtual bool SetDShaderProgram(VSDShader*	pDShaderProgram);
		virtual bool SetHShaderProgram(VSHShader*	pHShaderProgram);
		virtual bool SetCShaderProgram(VSCShader*	pCShaderProgram);
		//Load Release
		virtual bool OnLoadVShaderProgram(VSVShader * pVShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadPShaderProgram(VSPShader* pPShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadGShaderProgram(VSGShader * pGShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadDShaderProgram(VSDShader * pDShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadHShaderProgram(VSHShader * pHShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadCShaderProgram(VSCShader * pCShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadTexture(VSTexture * pTexture, VSResourceIdentifier *&pID);

		virtual bool OnLoadDepthStencil(VSDepthStencil * pDepthStencil, VSResourceIdentifier *&pID);

		virtual bool OnLoadRenderTarget(VSRenderTarget * pRenderTarget, VSResourceIdentifier *&pID);

		virtual bool OnLoadUnorderAccess(VSBufferUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID);
		virtual bool OnLoadUnorderAccess(VSTextureUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID);

		virtual bool OnLoadVBufferData(VSVertexBuffer * pVBuffer, VSResourceIdentifier *&pID);

		virtual bool OnLoadVBufferFormat(VSVertexFormat * pVertexFormat, VSResourceIdentifier *&pID);

		virtual bool OnLoadIBuffer(VSIndexBuffer* pIBuffer, VSResourceIdentifier *&pID);

		virtual bool OnLoadSBuffer(VSBufferResource* pSBuffer, VSResourceIdentifier *&pID);

		virtual bool OnLoadBlendState(VSBlendState * pBlendState, VSResourceIdentifier *&pID);

		virtual bool OnLoadDepthStencilState(VSDepthStencilState * pDepthStencilState, VSResourceIdentifier *&pID);

		virtual bool OnLoadRasterizerState(VSRasterizerState * pRasterizerState, VSResourceIdentifier *&pID);

		virtual bool OnLoadSamplerState(VSSamplerState * pSamplerState, VSResourceIdentifier *&pID);

		virtual bool OnLoadQuery(VSQuery * pQuery, VSResourceIdentifier *&pID);

		virtual uint32 SetVertexFormat(VSVertexFormat * pVertexFormat);

		virtual void * Lock(VSVertexBuffer * pVertexBuffer);
		virtual void UnLock(VSVertexBuffer * pVertexBuffer);

		virtual void * Lock(VSIndexBuffer * pIndexBuffer);
		virtual void UnLock(VSIndexBuffer * pIndexBuffer);

		virtual void * Lock(VSBufferResource * pStructBuffer);
		virtual void UnLock(VSBufferResource * pStructBuffer);

		virtual void * Lock(VSTexture * pTexture, uint32 uiLevel, uint32 uiFace);
		virtual void UnLock(VSTexture * pTexture, uint32 uiLevel, uint32 uiFace);

		virtual void * Lock(VSRenderTarget * pRenderTarget);
		virtual void UnLock(VSRenderTarget * pRenderTarget);

		virtual void * Lock(VSUnorderAccess  * pUnorderAccess);
		virtual void UnLock(VSUnorderAccess  * pUnorderAccess);

		virtual void * Lock(VSDepthStencil * pDepthStencil);
		virtual void UnLock(VSDepthStencil * pDepthStencil);

		virtual bool GetData(VSQuery * pQuery, void * pData, uint32 uiSize);

		virtual bool CopyResourceBuffer(VS2DTexture * pSource, VSCubeTexture * pDest, uint32 uiFace);

	public:
		//shader function
		virtual VSString TexColorBuffer(const VSColorBuffer * pColorBuffer) const;
		virtual VSString Tex2D(const VS2DTexSampler * p2DTexSampler, uint32 uiShaderType) const;
		virtual VSString Tex1D(const VS1DTexSampler * p1DTexSampler, uint32 uiShaderType) const;
		virtual VSString Mul(const VSMul * pMul) const;
		virtual VSString Div(const VSDiv * pDiv) const;
		virtual VSString Dot(const VSDot * pDot) const;
		virtual VSString Length(const VSLength * pLength) const;
		virtual VSString Sub(const VSSub * pSub) const;
		virtual VSString Add(const VSAdd * pAdd) const;
		virtual VSString MakeValue(const VSMakeValue * pMakeValue) const;
		virtual VSString Saturate(const VSSaturate * pSaturate) const;

		virtual VSString FloatByChannelNum(uint32 ChannelNum)const;
		virtual VSString Float()const;
		virtual VSString Float2()const;
		virtual VSString Float3()const;
		virtual VSString Float4()const;
		virtual VSString Return()const;

		virtual VSString FloatConst(const VSString & Value1)const;
		virtual VSString Float2Const(const VSString & Value1, const VSString & Value2)const;
		virtual VSString Float3Const(const VSString & Value1,
			const VSString & Value2, const VSString & Value3)const;
		virtual VSString Float4Const(const VSString & Value1, const VSString & Value2,
			const VSString & Value3, const VSString & Value4)const;

		virtual void CustomPointLightUseString(VSString & PointLightUseString, VSString & PointAttenuationDivString);

		virtual void CustomSpotLightUseString(VSString & SpotLightUseString, VSString & SpotAttenuationDivString);


		virtual VSString GetDeclareValue(const VSString &ShowName, uint32 uiValueType, uint32 uiRegisterIndex)const;
		virtual VSString GetDeclareSampler(const VSString &ShowName, uint32 SamplerType, uint32 uiRegisterIndex, uint32 SamplerSource)const;
		virtual void GetDeclareShareSampler(uint32 SamplerSource, uint32 uiRegisterIndex, VSString & OutString)const;
		//shader custom function
		virtual void CreateVInputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateVOutputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateVUserConstant(VSVShader * pVShader, MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateVFunction(MaterialShaderPara &MSPara,
			VSString & OutString);

		virtual void SetMaterialVShaderConstant(MaterialShaderPara &MSPara, VSVShader *pVShader);

		virtual void CreatePInputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreatePOutputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreatePUserConstant(VSPShader * pPShader, MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreatePFunction(MaterialShaderPara &MSPara,
			VSString & OutString);

		virtual void SetMaterialPShaderConstant(MaterialShaderPara &MSPara, VSPShader *pPShader);

		virtual void CreateGInputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateGOutputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateGUserConstant(VSGShader * pGShader, MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateGFunction(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void SetMaterialGShaderConstant(MaterialShaderPara &MSPara, VSGShader *pGShader);

		virtual void CreateHInputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateHOutputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateHUserConstant(VSHShader * pHShader, MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateHFunction(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void SetMaterialHShaderConstant(MaterialShaderPara &MSPara, VSHShader * pHShader);


		virtual void CreateDInputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateDOutputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateDUserConstant(VSDShader * pDShader, MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateDFunction(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void SetMaterialDShaderConstant(MaterialShaderPara &MSPara, VSDShader *pDShader);

		virtual void GetIncludeShader(VSString &OutString);

		virtual void GetDynamicShader(VSString &OutString);


		virtual void SkyLight(const VSString &WorldNormal, const VSString &UpColor, const VSString & DownColor, const VSString & DiffuseColor, VSString & OutString);
		virtual void DirectionalLight(int32 iLightNum, const VSString &Diffuse, const VSString &Specular, const VSString &SpecularPow,
			const VSString &WorldNormal, const VSString &WorldCameraDir, VSArray<VSString> ShadowString,
			VSString & OutString)const;
		virtual void CustomDirectionalLight(int32 iLightNum, const VSString &CustomString, VSArray<VSString> ShadowString, VSString & OutString)const;

		virtual void OrenNayarDirectionalLight(int32 iLightNum, const VSString &Diffuse, const VSString &RoughnessSquared,
			const VSString &WorldNormal, const VSString &WorldCameraDir, bool bLookUpTable, VSArray<VSString> ShadowString,
			VSString & OutString)const;
		

		virtual void PointLight(int32 iLightNum, const VSString &Diffuse, const VSString &Specular, const VSString &SpecularPow,
			const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, VSArray<VSString> ShadowString,
			VSString & OutString)const;
		virtual void CustomPointLight(int32 iLightNum, const VSString &CustomString, const VSString & WorldPos, VSArray<VSString> ShadowString, VSString & OutString)const;

		virtual void OrenNayarPointLight(int32 iLightNum, const VSString &Diffuse, const VSString &RoughnessSquared,
			const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, bool bLookUpTable, VSArray<VSString> ShadowString,
			VSString & OutString)const;

		virtual void SpotLight(int32 iLightNum, const VSString &Diffuse, const VSString &Specular, const VSString &SpecularPow,
			const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, VSArray<VSString> ShadowString,
			VSString & OutString)const;
		virtual void CustomSpotLight(int32 iLightNum, const VSString &CustomString, const VSString & WorldPos, VSArray<VSString> ShadowString, VSString & OutString)const;
		virtual void OrenNayarSpotLight(int32 iLightNum, const VSString &Diffuse, const VSString &RoughnessSquared,
			const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, bool bLookUpTable, VSArray<VSString> ShadowString,
			VSString & OutString)const;

		virtual void LocalToWorldPos(const VSString & LocalPos, VSString &OutString)const;
		virtual void WorldToViewPos(const VSString & LocalPos, VSString &OutString)const;

		virtual void LocalToWorldNormal(const VSString & LocalNormal, VSString &OutString)const;

		virtual void WorldToViewNormal(const VSString & WorldNormal, VSString &OutString)const;

		virtual void GetWorldViewDir(VSString &OutString)const;

		virtual void TransProjPos(const VSString & Pos, const VSString & Matrix, VSString &OutString)const;
		virtual void BumpNormal(const VSString &TexNormal, VSString &OutString)const;

		virtual void ComputeDir(const VSString & Orgin, const VSString & End, VSString &OutString)const;
		virtual void ComputeLength(const VSString & Orgin, const VSString & End, VSString &OutString)const;
		virtual void ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight, const VSString &BoneVector,
			const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const;
		virtual void ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight,
			const VSString &AnimDataResource, const VSString & AnimAllInfo,
			const VSString &AnimInstanceInfo, const VSString &AnimNum,
			const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const;

		virtual void BoneTranPos(const VSString & LocalPos, const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const;
		virtual void BoneTranNormal(const VSString & LocalNormal, const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const;

		virtual void DecodeNormal1(const VSString & Normal, VSString &OutString) const;
		virtual void RecodeNormal1(const VSString & Normal, VSString &OutString) const;
		virtual void DecodeNormal2(const VSString & Normal, const VSString & Tengent, VSString &OutString) const;

		virtual void DecodeNormal3(const VSString & Normal, VSString &OutString) const;
		virtual void RecodeNormal3(const VSString & Normal, VSString &OutString) const;

		virtual void Reciprocal(const VSString & Value, VSString &OutString)const;
		virtual void Clip(const VSString & ClipValue, VSString &OutString)const;
		virtual void GreaterZeroPow(const VSString & Value, float Exp, VSString &OutString) const;
		virtual void SaturatePow(const VSString & Value, float Exp, VSString &OutString) const;
		virtual void Saturate(const VSString & Value, VSString &OutString)const;

		virtual void PointLightCubeShadow(const VSString & PointLightName, const VSString & CubeShadowSampler, const VSString & ViewMatrix,
			const VSString & WorldPos, VSString& OutString)const;

		virtual void PointLightCubeShadow(const VSString & PointLightName, const VSString & CubeShadowSampler,
			const VSString & WorldPos, VSString& OutString)const;

		virtual void PointLightVolumeShadow(const VSString & PointLightName, const VSString & VolumeShadowSampler,
			const VSString & WorldPos, VSString& OutString)const;

		virtual void PointLightDualParaboloidShadow(const VSString & PointLightName, const VSString & ShadowSampler,
			const VSString & WorldPos, VSString& OutString)const;
		virtual void DirectionLightVolumeShadow(const VSString & DirLightName, const VSString & VolumeShadowSampler,
			const VSString & WorldPos, VSString& OutString)const;

		virtual void DirectionLightCSMShadow(const VSString & DirLightName, const VSString & ShadowSampler,
			const VSString & WorldPos, const VSString & ViewPos, const VSString &FarZ, VSString& OutString)const;

		virtual void DirectionLightShadow(const VSString & DirLightName, const VSString & ShadowSampler,
			const VSString & WorldPos, VSString& OutString)const;

		virtual void SpotLightShadow(const VSString & SpotLightName, const VSString & ShadowSampler,
			const VSString & WorldPos, VSString& OutString)const;

		virtual void TranLightToTemp(VSArray<VSLight *> LightArray, VSString & OutString)const;

		virtual void GetLightFunction(VSArray<VSLight *> LightArray, const VSString & WorldPos, VSString & OutString)const;

		virtual void EncodeReflect(const VSString & Mip, const VSString & Pow, VSString& OutString);

		virtual void DecodeReflect(const VSString & Mip, const VSString & Pow, const VSString & Value, VSString& OutString);

	};
	
}

