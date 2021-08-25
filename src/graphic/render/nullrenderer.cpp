#include "graphic/render/nullrenderer.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
VSNullRenderer::VSNullRenderer()
{
	
}
VSNullRenderer::~VSNullRenderer()
{
	ReleaseDefaultValue();
}
void VSNullRenderer::ClearBuffers(uint32 uiClearFlag){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ClearBackBuffer(){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ClearZBuffer(){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ClearStencilBuffer(){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ClearBuffers(){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ClearBuffers(uint32 uiClearFlag, int32 iXPos, int32 iYPos, int32 iWidth,
	int32 iHeight){
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::ClearBackBuffer(int32 iXPos, int32 iYPos, int32 iWidth,
	int32 iHeight){
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::ClearZBuffer(int32 iXPos, int32 iYPos, int32 iWidth,
	int32 iHeight){
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::ClearStencilBuffer(int32 iXPos, int32 iYPos, int32 iWidth,
	int32 iHeight){
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::ClearBuffers(int32 iXPos, int32 iYPos, int32 iWidth,
	int32 iHeight){
	VSMAC_ASSERT(0); return;
}
bool VSNullRenderer::SetViewPort(VSViewPort *pViewPort){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::UseWindow(int32 uiWindowID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::CooperativeLevel(){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::BeginRendering(){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::EndRendering(){VSMAC_ASSERT(0);return 0;}

void VSNullRenderer::DeviceLost(){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ResetDevice(){VSMAC_ASSERT(0);return ;}
bool VSNullRenderer::ChangeScreenSize(uint32 uiWidth, uint32 uiHeight, bool bWindow){VSMAC_ASSERT(0);return 0 ;}
bool VSNullRenderer::DrawMesh(VSGeometry * pGeometry, VSRenderState * pRenderState, VSVShader * pVShader, VSPShader * pPShader, 
	VSGShader * pGShader, VSHShader * pHShader, VSDShader * pDShader){
	VSMAC_ASSERT(0); return 0;
}
bool VSNullRenderer::DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4]){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer, uint32 uiVertexNum,
	MATH_USHORT_INDEX * pIndexBuffer, uint32 uiIndexNum){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, uint32 uiVertexNum,
	MATH_USHORT_INDEX * pIndexBuffer, uint32 uiIndexNum){
	VSMAC_ASSERT(0); return 0;
}
const VSAdapterInfo* VSNullRenderer::GetAdapterInfo(uint32 & uiAdapterNum)
{
	VSMAC_ASSERT(0); return NULL;
}
uint32 VSNullRenderer::SetBlendState(VSBlendState * pBlendState, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetDepthStencilState(VSDepthStencilState * pDepthStencilState, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetRasterizerState(VSRasterizerState * pRasterizerState, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
void VSNullRenderer::SetClipPlane(VSArray<Plane> &Plane, bool bEnable){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::SetScissorRect(VSArray<VSRect2> &Rect, bool bEnable){VSMAC_ASSERT(0);return ;}

uint32 VSNullRenderer::SetVBuffer(VSVertexBuffer * pVBuffer, uint32 uiStream){ VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetIBuffer(VSIndexBuffer* pIBuffer){VSMAC_ASSERT(0);return 0;}
uint32 VSNullRenderer::BeginQuery(VSQuery * pQuery) { VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::EndQuery(VSQuery * pQuery) { VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetPTexture(VSTexture* pTexture, uint32 i){VSMAC_ASSERT(0);return 0;}
uint32 VSNullRenderer::SetVTexture(VSTexture* pTexture, uint32 i){VSMAC_ASSERT(0);return 0;}
uint32 VSNullRenderer::SetGTexture(VSTexture* pTexture, uint32 i){ VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetDTexture(VSTexture* pTexture, uint32 i){ VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetHTexture(VSTexture* pTexture, uint32 i){ VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetCTexture(VSTexture* pTexture, uint32 i) { VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetCBufferResource(VSBufferResource* pBufferResource, uint32 i) { VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetVBufferResource(VSBufferResource* pBufferResource, uint32 i) { VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetPBufferResource(VSBufferResource* pBufferResource, uint32 i) { VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetGBufferResource(VSBufferResource* pBufferResource, uint32 i) { VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetHBufferResource(VSBufferResource* pBufferResource, uint32 i) { VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetDBufferResource(VSBufferResource* pBufferResource, uint32 i) { VSMAC_ASSERT(0); return 0; }
void VSNullRenderer::Dispath(uint32 uiThreadGroupCountX, uint32 uiThreadGroupCountY,
	uint32 uiThreadGroupCountZ) 
{
	VSMAC_ASSERT(0); return;
}
uint32 VSNullRenderer::SetPSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetVSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetGSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetDSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetHSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
uint32 VSNullRenderer::SetCSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet) { VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::SetDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::EndDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer){VSMAC_ASSERT(0);return 0;}
void VSNullRenderer::SetVShaderConstant(VSVShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetVShaderBindResource(VSVShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetPShaderConstant(VSPShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetPShaderBindResource(VSPShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetGShaderConstant(VSGShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetGShaderBindResource(VSGShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetDShaderConstant(VSDShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetDShaderBindResource(VSDShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetHShaderConstant(VSHShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetHShaderBindResource(VSHShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetCShaderConstant(VSCShader * pShader) { VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetCShaderBindResource(VSCShader * pShader) { VSMAC_ASSERT(0); return; }


bool VSNullRenderer::SetVShaderProgram(VSVShader*	pVShaderProgram){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::SetPShaderProgram(VSPShader*pPShaderProgram){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::SetGShaderProgram(VSGShader*pGShaderProgram){ VSMAC_ASSERT(0); return 0; }
bool VSNullRenderer::SetDShaderProgram(VSDShader*	pDShaderProgram){ VSMAC_ASSERT(0); return 0; }
bool VSNullRenderer::SetHShaderProgram(VSHShader*	pHShaderProgram){ VSMAC_ASSERT(0); return 0; }
bool VSNullRenderer::SetCShaderProgram(VSCShader*	pCShaderProgram) { VSMAC_ASSERT(0); return 0; }
//Load Release
bool VSNullRenderer::OnLoadVShaderProgram(VSVShader * pVShaderProgram, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadPShaderProgram(VSPShader* pPShaderProgram, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadGShaderProgram(VSGShader* pGShaderProgram, VSResourceIdentifier *&pID){ VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::OnLoadDShaderProgram(VSDShader * pDShaderProgram, VSResourceIdentifier *&pID){ VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::OnLoadHShaderProgram(VSHShader * pHShaderProgram, VSResourceIdentifier *&pID){ VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::OnLoadCShaderProgram(VSCShader * pCShaderProgram, VSResourceIdentifier *&pID) { VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::OnLoadTexture(VSTexture * pTexture, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadDepthStencil(VSDepthStencil * pDepthStencil, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadRenderTarget(VSRenderTarget * pRenderTarget, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadUnorderAccess(VSBufferUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID) { VSMAC_ASSERT(0); return 0; }
bool VSNullRenderer::OnLoadUnorderAccess(VSTextureUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID) { VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::OnLoadVBufferData(VSVertexBuffer * pVBuffer, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadVBufferFormat(VSVertexFormat * pVertexFormat, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadIBuffer(VSIndexBuffer* pIBuffer, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadSBuffer(VSBufferResource* pSBuffer, VSResourceIdentifier *&pID) { VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::OnLoadBlendState(VSBlendState * pBlendState, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadDepthStencilState(VSDepthStencilState * pDepthStencilState, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadRasterizerState(VSRasterizerState * pRasterizerState, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadSamplerState(VSSamplerState * pSamplerState, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadQuery(VSQuery * pQuery, VSResourceIdentifier *&pID) { VSMAC_ASSERT(0); return 0; }

uint32 VSNullRenderer::SetVertexFormat(VSVertexFormat * pVertexFormat){VSMAC_ASSERT(0);return 0;}

void * VSNullRenderer::Lock(VSVertexBuffer * pVertexBuffer){VSMAC_ASSERT(0);return NULL;}
void VSNullRenderer::UnLock(VSVertexBuffer * pVertexBuffer){VSMAC_ASSERT(0);return ;}

void * VSNullRenderer::Lock(VSIndexBuffer * pIndexBuffer){ VSMAC_ASSERT(0); return NULL; }
void VSNullRenderer::UnLock(VSIndexBuffer * pIndexBuffer){VSMAC_ASSERT(0);return ;}

void * VSNullRenderer::Lock(VSBufferResource * pStructBuffer) { VSMAC_ASSERT(0); return NULL; }
void VSNullRenderer::UnLock(VSBufferResource * pStructBuffer) { VSMAC_ASSERT(0); return; }

void * VSNullRenderer::Lock(VSTexture * pTexture, uint32 uiLevel, uint32 uiFace){ VSMAC_ASSERT(0); return NULL; }
void VSNullRenderer::UnLock(VSTexture * pTexture, uint32 uiLevel, uint32 uiFace){VSMAC_ASSERT(0);return ;}

bool VSNullRenderer::CopyResourceBuffer(VS2DTexture * pSource, VSCubeTexture * pDest, uint32 uiFace){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::GetData(VSQuery * pQuery, void * pData, uint32 uiSize) { VSMAC_ASSERT(0); return NULL; };

void * VSNullRenderer::Lock(VSRenderTarget * pRenderTarget) { VSMAC_ASSERT(0); return NULL; };
void VSNullRenderer::UnLock(VSRenderTarget * pRenderTarget) { VSMAC_ASSERT(0); return; };

void * VSNullRenderer::Lock(VSUnorderAccess  * pUnorderAccess) { VSMAC_ASSERT(0); return NULL; };
void VSNullRenderer::UnLock(VSUnorderAccess  * pUnorderAccess) { VSMAC_ASSERT(0); return; };

void * VSNullRenderer::Lock(VSDepthStencil * pDepthStencil) { VSMAC_ASSERT(0); return NULL; };
void VSNullRenderer::UnLock(VSDepthStencil * pDepthStencil) { VSMAC_ASSERT(0); return; };

VSString VSNullRenderer::Tex2D(const VS2DTexSampler * p2DTexSampler, uint32 uiShaderType) const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Tex1D(const VS1DTexSampler * p1DTexSampler, uint32 uiShaderType) const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::TexColorBuffer(const VSColorBuffer * pColorBuffer) const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Mul(const VSMul * pMul) const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Saturate(const VSSaturate * pSaturate) const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Div(const VSDiv * pDiv) const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Dot(const VSDot * pDot) const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Length(const VSLength * pLength) const { VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Sub(const VSSub * pSub) const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Add(const VSAdd * pAdd) const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::MakeValue(const VSMakeValue * pMakeValue)const{ VSMAC_ASSERT(0); return ""; }

VSString VSNullRenderer::FloatByChannelNum(uint32 ChannelNum)const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Float()const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Float2()const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Float3()const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Float4()const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Return()const{ VSMAC_ASSERT(0); return ""; }

VSString VSNullRenderer::GetDeclareValue(const VSString &ShowName, uint32 uiValueType, uint32 uiRegisterIndex)const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::GetDeclareSampler(const VSString &ShowName, uint32 SamplerType, uint32 uiRegisterIndex, uint32 SamplerSource)const{ VSMAC_ASSERT(0); return ""; }
void VSNullRenderer::GetDeclareShareSampler(uint32 SamplerSource, uint32 uiRegisterIndex, VSString & OutString)const
{
	VSMAC_ASSERT(0);
	return;
}
VSString VSNullRenderer::FloatConst(const VSString & Value1)const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Float2Const(const VSString & Value1, const VSString & Value2)const{ VSMAC_ASSERT(0); return ""; }
VSString VSNullRenderer::Float3Const(const VSString & Value1,
	const VSString & Value2, const VSString & Value3)const{
	VSMAC_ASSERT(0); return "";
}
VSString VSNullRenderer::Float4Const(const VSString & Value1, const VSString & Value2,
	const VSString & Value3, const VSString & Value4)const{
	VSMAC_ASSERT(0); return "";
}

void VSNullRenderer::CustomPointLightUseString(VSString & PointLightUseString, VSString & PointAttenuationDivString){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::CustomSpotLightUseString(VSString & SpotLightUseString, VSString & SpotAttenuationDivString){VSMAC_ASSERT(0);return ;}




//shader custom function
void VSNullRenderer::CreateVInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateVOutputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateVUserConstant(VSVShader * pVShader, MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateVFunction(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::SetMaterialVShaderConstant(MaterialShaderPara &MSPara,VSVShader *pVShader){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::CreatePInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreatePOutputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreatePUserConstant(VSPShader * pPShader, MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreatePFunction(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::SetMaterialPShaderConstant(MaterialShaderPara &MSPara,VSPShader *pPShader){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::CreateGInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateGOutputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateGUserConstant(VSGShader * pGShader, MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateGFunction(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::SetMaterialGShaderConstant(MaterialShaderPara &MSPara, VSGShader *pGShader){ VSMAC_ASSERT(0); return; }

void VSNullRenderer::CreateHInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString) {VSMAC_ASSERT(0); return;}
void VSNullRenderer::CreateHOutputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateHUserConstant(VSHShader * pHShader, MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateHFunction(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::SetMaterialHShaderConstant(MaterialShaderPara &MSPara, VSHShader * pHShader) { VSMAC_ASSERT(0); return; }

void VSNullRenderer::CreateDInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString) {
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::CreateDOutputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString) {
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::CreateDUserConstant(VSDShader * pDShader, MaterialShaderPara &MSPara,
	VSString & OutString) {
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::CreateDFunction(MaterialShaderPara &MSPara,
	VSString & OutString) {
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::SetMaterialDShaderConstant(MaterialShaderPara &MSPara, VSDShader *pDShader) { VSMAC_ASSERT(0); return; }

void VSNullRenderer::GetIncludeShader(VSString &OutString){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::GetDynamicShader(VSString &OutString){VSMAC_ASSERT(0);return ;}


void VSNullRenderer::SkyLight(const VSString &WorldNormal, const VSString &UpColor, const VSString & DownColor, const VSString & DiffuseColor, VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::DirectionalLight(int32 iLightNum, const VSString &Diffuse, const VSString &Specular, const VSString &SpecularPow,
	const VSString &WorldNormal, const VSString &WorldCameraDir, VSArray<VSString> ShadowString,
	VSString & OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CustomDirectionalLight(int32 iLightNum, const VSString &CustomString, VSArray<VSString> ShadowString, VSString & OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::OrenNayarDirectionalLight(int32 iLightNum, const VSString &Diffuse, const VSString &RoughnessSquared,
	const VSString &WorldNormal, const VSString &WorldCameraDir, bool bLookUpTable, VSArray<VSString> ShadowString,
	VSString & OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::PointLight(int32 iLightNum, const VSString &Diffuse, const VSString &Specular, const VSString &SpecularPow,
	const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, VSArray<VSString> ShadowString,
	VSString & OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CustomPointLight(int32 iLightNum, const VSString &CustomString, const VSString & WorldPos, VSArray<VSString> ShadowString, VSString & OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::OrenNayarPointLight(int32 iLightNum, const VSString &Diffuse, const VSString &RoughnessSquared,
	const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, bool bLookUpTable, VSArray<VSString> ShadowString,
	VSString & OutString)const{VSMAC_ASSERT(0);return ;}


void VSNullRenderer::SpotLight(int32 iLightNum, const VSString &Diffuse, const VSString &Specular, const VSString &SpecularPow,
	const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, VSArray<VSString> ShadowString,
	VSString & OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CustomSpotLight(int32 iLightNum, const VSString &CustomString, const VSString & WorldPos, VSArray<VSString> ShadowString, VSString & OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::OrenNayarSpotLight(int32 iLightNum, const VSString &Diffuse, const VSString &RoughnessSquared,
	const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, bool bLookUpTable, VSArray<VSString> ShadowString,
	VSString & OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::LocalToWorldPos(const VSString & LocalPos, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::WorldToViewPos(const VSString & LocalPos, VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::LocalToWorldNormal(const VSString & LocalNormal, VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::WorldToViewNormal(const VSString & WorldNormal, VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::GetWorldViewDir(VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::TransProjPos(const VSString & Pos, const VSString & Matrix, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::BumpNormal(const VSString &TexNormal, VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::ComputeDir(const VSString & Orgin, const VSString & End, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ComputeLength(const VSString & Orgin, const VSString & End, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight, const VSString &BoneVector,
	const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight,
	const VSString &AnimDataResource, const VSString & AnimAllInfo,
	const VSString &AnimInstanceInfo, const VSString &AnimNum,
	const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const
{
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::BoneTranPos(const VSString & LocalPos, const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::BoneTranNormal(const VSString & LocalNormal, const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::DecodeNormal1(const VSString & Normal, VSString &OutString) const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::RecodeNormal1(const VSString & Normal, VSString &OutString) const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::DecodeNormal2(const VSString & Normal, const VSString & Tengent, VSString &OutString) const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::DecodeNormal3(const VSString & Normal, VSString &OutString) const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::RecodeNormal3(const VSString & Normal, VSString &OutString) const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::Reciprocal(const VSString & Value, VSString &OutString)const{ VSMAC_ASSERT(0); return; }
void VSNullRenderer::Clip(const VSString & ClipValue, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::GreaterZeroPow(const VSString & Value, float Exp, VSString &OutString) const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::SaturatePow(const VSString & Value, float Exp, VSString &OutString) const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::Saturate(const VSString & Value, VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::PointLightCubeShadow(const VSString & PointLightName, const VSString & CubeShadowSampler, const VSString & ViewMatrix,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::PointLightCubeShadow(const VSString & PointLightName, const VSString & CubeShadowSampler,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::PointLightVolumeShadow(const VSString & PointLightName, const VSString & VolumeShadowSampler,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::PointLightDualParaboloidShadow(const VSString & PointLightName, const VSString & ShadowSampler,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::DirectionLightVolumeShadow(const VSString & DirLightName, const VSString & VolumeShadowSampler,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::DirectionLightCSMShadow(const VSString & DirLightName, const VSString & ShadowSampler,
	const VSString & WorldPos, const VSString & ViewPos, const VSString &FarZ, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::DirectionLightShadow(const VSString & DirLightName, const VSString & ShadowSampler,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::SpotLightShadow(const VSString & SpotLightName, const VSString & ShadowSampler,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::TranLightToTemp(VSArray<VSLight *> LightArray, VSString & OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::GetLightFunction(VSArray<VSLight *> LightArray, const VSString & WorldPos, VSString & OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::EncodeReflect(const VSString & Mip, const VSString & Pow, VSString& OutString){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::DecodeReflect(const VSString & Mip, const VSString & Pow, const VSString & Value, VSString& OutString){VSMAC_ASSERT(0);return ;}
uint32 VSNullRenderer::GetTextureCompressLen(uint32 uiLen)
{
	return Max((unsigned int)1, (unsigned int)(uiLen / 4));
}
