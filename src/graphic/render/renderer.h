#pragma once


#include "math/vector2.h"
#include "math/vector3w.h"
#include "math/rect2.h"
#include "graphic/render/renderstate/renderstate.h"
#include "graphic/render/renderstate/samplerstate.h"
#include "graphic/material/shader/shaderkey.h"
#include "graphic/node/nodecomponent/camera.h"
#include "graphic/core/profiler.h"


namespace zq{


	EXTERN_TIME_PROFILENODE(DrawMesh)
	EXTERN_COUNT_PROFILENODE(DrawPrimitiveNum)
	EXTERN_COUNT_PROFILENODE(DrawGeometryNum)
	class VSCamera;
	class VSBind;
	class VSFont;
	class VSCamera;
	class VSTexAllState;
	class VSGeometry;
	class VSSpatial;
	class VS2DTexture;
	class VS3DTexture;
	class VSMaterial;
	class VSMaterialInstance;
	class VSMeshData;
	class VS2DTexSampler;
	class VS1DTexSampler;
	class VSPutNode;
	class VSLight;
	class VSInstanceGeometry;
	class VSIndexBuffer;
	class VSBufferResource;
	class VSMaterialInstance;
	class VSMul;
	class VSLength;
	class VSDiv;
	class VSDot;
	class VSSub;
	class VSAdd;
	class VSColorBuffer;
	class VSMakeValue;
	class VSSaturate;
	class VSSlot;
	DECLARE_Ptr(VSDVGeometry);
	DECLARE_Ptr(VSTexture);
	DECLARE_Ptr(VSVertexFormat);
	DECLARE_Proxy(VSFont);
	typedef struct VSDISPLAYMODE_STURCT
	{
		UINT            uiWidth;			//显示分辨率
		UINT            uiHeight;
		UINT            uiRefreshRate;		//刷新率
		UINT			uiBitFormat;			
		VSString			StringExpress;		//字符串表示
	}VSDisplayMode, *VSDisplayModePtr;

	typedef struct VSDEVICEINFO_STURCT
	{
		UINT		uiMaxMultisample;
		UINT		uiMaxTextureWidth;
		UINT		uiMaxTextureHeight;
		UINT		uiMaxUseClipPlane;
		UINT		uiMaxTexture;
		UINT		fMaxAnisotropy;
		UINT		VertexShaderVersion;
		UINT		PixelShaderVersion;
		UINT		uiMaxRTNum;	
	}VSDeviceInfo,*VSDeviceInfoPtr;

	typedef struct VSADAPTERINFO_STURCT
	{
		unsigned int		m_uiAdapter;
		VSString			AdapterName;					
		VSArray<VSDisplayMode>	DisplayMode;	
		UINT			uiDisplayModeNum;
		//HAL SW REF
		VSDeviceInfo	DeviceInfo[3];	
	}VSAdapterInfo,*VSAdapterInfoPtr;

	class GRAPHIC_API VSRenderer
	{
	public:
		VSRenderer();
		virtual ~VSRenderer() = 0;
		friend class VSVertexBuffer;
		friend class VSVertexFormat;
		friend class VSDataBuffer;
		friend class VSDepthStencil;
		friend class VSTexture;
		friend class VSRenderTarget;
		friend class VSBufferUnorderAccess;
		friend class VSUnorderAccess;
		friend class VSTextureUnorderAccess;
		friend class VSIndexBuffer;
		friend class VSBufferResource;
		friend class VSCustomMaterial;
		friend class VSVShader;
		friend class VSPShader;
		friend class VSGShader;
		friend class VSDShader;
		friend class VSHShader;
		friend class VSCShader;
		friend class VSCubeTexture;
		friend class VSSamplerState;
		friend class VSQuery;
		friend class VSBlendState;
		friend class VSDepthStencilState;
		friend class VSRasterizerState;
		friend class VSRenderThreadSys;
		friend class VSBind;
		enum	//Render API Type
		{
			RAT_NULL,
			RAT_DIRECTX9,
			RAT_DIRECTX11,
			RAT_MAX
		};
		enum //function return information
		{
			FRI_FAIL,		
			FRI_SUCCESS,
			FRI_SAMERESOURCE
		};
		enum //Multisample
		{
			MS_NONE,
			MS_2,
			MS_4,
			MS_8,
			MS_16,
			MS_MAX
		};
		enum	//Surface Format Type
		{
			SFT_A8R8G8B8,
			SFT_X8R8G8B8,
			SFT_D16,                   
			SFT_A32B32G32R32F, 
			SFT_A16B16G16R16F, 
			SFT_G16R16F, 
			SFT_R16F, 
			SFT_R32F,
			SFT_UNKNOWN,
			SFT_D24S8,
			SFT_G32R32F,
			SFT_R5G6B5,
			SFT_A8,
			SFT_G16R16,
			SFT_A16B16G16R16,
			SFT_R8,
			SFT_A8R8G8B8_S,
			SFT_G16R16_S,
			SFT_R16,
			SFT_A16B16G16R16_S,
			//Compress Type
			SFT_BC1,
			SFT_BC2,	
			SFT_BC3,
			SFT_BC4,
			SFT_BC5,
			SFT_MAX
		};

        enum //Clear Flag
        {
            CF_NONE = 0,
            CF_COLOR = 1 << 0,
            CF_DEPTH = 1 << 1,
            CF_STENCIL = 1 << 2,
            CF_COLOR_DEPTH = CF_COLOR | CF_DEPTH,
            CF_COLOR_STENCIL = CF_COLOR | CF_STENCIL,
            CF_DEPTH_STENCIL = CF_DEPTH | CF_STENCIL,
            CF_USE_ALL = CF_COLOR | CF_DEPTH | CF_STENCIL
        };
		

        inline uint32 ShaderVersion(UINT Major, UINT Minor)
        {
            return (0xFFFE0000 | ((Major) << 8) | (Minor));
        }

        inline uint32 GetScreenWith()const
        {
            return m_uiScreenWidth;
        }

        inline uint32 GetScreenHeight()const
        {
            return m_uiScreenHeight;
        }

        inline uint32 GetCurAnisotropy()const
        {
            return m_uiCurAnisotropy;
        }

        inline uint32 GetCurMultisample()const
        {
            return m_uiCurMultisample;
        }

        inline uint32 GetMaxMultisample()const
        {
            return m_uiMaxMultisample;
        }

        inline uint32 GetDepthStencilFormat()const
        {
            return m_uiDepthStencilFormat;
        }

        inline bool IsWindowed()const
        {
            return m_bWindowed;
        }

        inline uint32 WindoweNum()const
        {
            if (m_bWindowed)
            {
                return m_iNumChildWindow;
            }

            return 0;
        }

        inline uint32 GetMaxTextureWidth()const
        {
            return m_uiMaxTextureWidth;
        }

        inline uint32 GetMaxTextureHeight()const
        {
            return m_uiMaxTextureHeight;
        }

        inline uint32 GetMaxUseClipPlane()const
        {
            return m_uiMaxUseClipPlane;
        }

        inline uint32 GetMaxBindResource(uint32 uiShaderType)const
        {
            return m_uiMaxBindResource[uiShaderType];
        }

        inline uint32 GetMaxSampler(uint32 uiShaderType)const
        {
            return m_uiMaxSampler[uiShaderType];
        }

        inline uint32 GetBufferFormat()const
        {
            return m_uiBufferFormat;
        }

        inline uint32 GetMaxAnisotropy()const
        {
            return m_uiMaxAnisotropy;
        }

        inline uint32 GetMaxRTNum()const
        {
            return m_uiMaxRTNum;
        }

        inline uint32 GetMaxUAVNum()const
        {
            return m_uiMaxUAVNum;
        }

        inline VSString GetAdapterName()const
        {
            return m_AdapterName;
        }

        inline void SetClearColor(const VSColorRGBA& ClearColor)
        {
            m_ClearColor = ClearColor;
        }

        inline const VSColorRGBA& GetClearColor() const
        {
            return m_ClearColor;
        }

        inline void SetClearDepth(float fClearDepth)
        {
            m_fClearDepth = fClearDepth;
        }

        inline float GetClearDepth() const
        {
            return m_fClearDepth;
        }

        inline void SetClearStencil(uint32 uiClearStencil)
        {
            m_uiClearStencil = uiClearStencil;
        }

        inline uint32 GetClearStencil() const
        {
            return m_uiClearStencil;
        }

        inline uint32 GetCurRTWidth()const
        {
            return m_uiCurRTWidth;
        }

        inline uint32 GetCurRTHeight()const
        {
            return m_uiCurRTHeight;
        }

        inline uint32 GetCurRTMultisampler()const
        {
            return m_uiCurRTMultisample;
        }

        inline uint32 GetSuitableMultisample(uint32 Multisample)const
        {
            return Multisample > m_uiMaxMultisample ? m_uiMaxMultisample : Multisample;
        }


		virtual int32 GetRendererType () const = 0;
		virtual void ClearBuffers(uint32 uiClearFlag) = 0;
		virtual void ClearBackBuffer () = 0;
		virtual void ClearZBuffer () = 0;
		virtual void ClearStencilBuffer () = 0;
		virtual void ClearBuffers () = 0;
			
		virtual void ClearBuffers(uint32 uiClearFlag,int32 iXPos, int32 iYPos, int32 iWidth, int32 iHeight) = 0;
		virtual void ClearBackBuffer (int32 iXPos, int32 iYPos, int32 iWidth, int32 iHeight) = 0;
		virtual void ClearZBuffer (int32 iXPos, int32 iYPos, int32 iWidth, int32 iHeight) = 0;
		virtual void ClearStencilBuffer (int32 iXPos, int32 iYPos, int32 iWidth, int32 iHeight) = 0;
		virtual void ClearBuffers (int32 iXPos, int32 iYPos, int32 iWidth, int32 iHeight) = 0;

		void DrawText(int32 iX, int32 iY, const uint32 rColor,const char* acText, ...);		
		virtual bool SetViewPort(VSViewPort *pViewPort = NULL) = 0;
		
		void SetFont(VSFontR * pFont);

		virtual bool UseWindow(int32 uiWindowID = -1) = 0;
		
		virtual bool CooperativeLevel() = 0;
		virtual bool BeginRendering() = 0;
		virtual bool EndRendering() = 0;
		
		virtual void DeviceLost() = 0;
		virtual void ResetDevice() = 0;
		virtual bool ChangeScreenSize(uint32 uiWidth,uint32 uiHeight,bool bWindow) = 0;

		virtual const VSAdapterInfo* GetAdapterInfo(uint32 & uiAdapterNum) = 0;
		static uint32 GetBytesPerPixel(uint32 uiFormatType);
		static uint32 GetChannelPerPixel(uint32 uiFormatType);
		static uint32 GetMinDimension(uint32 uiFormatType);
		static const VSString & GetVShaderProgramMain();
		static const VSString & GetPShaderProgramMain();
		static const VSString & GetGShaderProgramMain();
		static const VSString & GetHShaderProgramMain();
		static const VSString & GetDShaderProgramMain();

		//Load Release
		bool LoadVShaderProgram (VSVShader * pVShaderProgram);
		bool ReleaseVShaderProgram(VSVShader * pVShaderProgram);

		bool LoadPShaderProgram (VSPShader * pPShaderProgram);
		bool ReleasePShaderProgram (VSPShader* pPShaderProgram);

		bool LoadGShaderProgram(VSGShader * pGShaderProgram);
		bool ReleaseGShaderProgram(VSGShader * pGShaderProgram);

		bool LoadDShaderProgram(VSDShader * pDShaderProgram);
		bool ReleaseDShaderProgram(VSDShader * pDShaderProgram);

		bool LoadHShaderProgram(VSHShader * pHShaderProgram);
		bool ReleaseHShaderProgram(VSHShader * pHShaderProgram);

		bool LoadCShaderProgram(VSCShader * pCShaderProgram);
		bool ReleaseCShaderProgram(VSCShader * pCShaderProgram);

		bool LoadTexture (VSTexture * pTexture);
		bool ReleaseTexture (VSTexture * pTexture);

		bool LoadBufferResource(VSBufferResource * pBufferResource);
		bool ReleaseBufferResource(VSBufferResource * pBufferResource);

		bool LoadVBuffer(VSVertexBuffer * pVBuffer);
		bool ReleaseVBuffer(VSVertexBuffer * pVBuffer);
		
		bool LoadIBuffer (VSIndexBuffer* pIBuffer);
		bool ReleaseIBuffer (VSIndexBuffer* pIBuffer);
	
		bool LoadDepthStencil (VSDepthStencil * pDepthStencil);
		bool ReleaseDepthStencil (VSDepthStencil * pDepthStencil);
		
		bool LoadRenderTarget (VSRenderTarget  * pRenderTarget);
		bool ReleaseRenderTarget  (VSRenderTarget * pRenderTarget);

		bool LoadUnorderAccess(VSUnorderAccess  * pUnorderAccess);
		bool ReleaseUnorderAccess(VSUnorderAccess  * pUnorderAccess);

		bool LoadBlendState (VSBlendState * pBlendState);
		bool ReleaseBlendState  (VSBlendState * pBlendState);

		bool LoadDepthStencilState (VSDepthStencilState * pDepthStencilState);
		bool ReleaseDepthStencilState  (VSDepthStencilState * pDepthStencilState);

		bool LoadRasterizerState(VSRasterizerState * pRasterizerState);
		bool ReleaseRasterizerState  (VSRasterizerState * pRasterizerState);

		bool LoadSamplerState(VSSamplerState * pSamplerState);
		bool ReleaseSamplerState  (VSSamplerState * pSamplerState);

		bool LoadQuery(VSQuery * pQuery);
		bool ReleaseQuery(VSQuery * pQuery);
		
		virtual bool DrawMesh(VSGeometry* pGeometry, VSRenderState* pRenderState, VSVShader* pVShader, VSPShader* pPShader, 
			VSGShader* pGShader, VSHShader* pHShader, VSDShader* pDShader) = 0;

		struct SCREEN_QUAD_TYPE
		{
			Vector3W Point;
			Vector2 UV;
		};

		struct SCREEN_FONT_TYPE
		{
			Vector3W Point;
			Vector2 UV;
			uint32 Color;
		};

		virtual bool DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4]) = 0;
		virtual bool DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer,uint32 uiVertexNum, MATH_USHORT_INDEX * pIndexBuffer,uint32 uiIndexNum) = 0;
		virtual bool DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, uint32 uiVertexNum,
			MATH_USHORT_INDEX * pIndexBuffer, uint32 uiIndexNum) = 0;
		void SetUseState(VSRenderState & RenderState,uint32 uiRenderStateInheritFlag);
		void ClearUseState();
		inline const VSRenderState & GetUseState() const
		{
			return m_UseState;
		}
		inline uint32 GetRenderStateInheritFlag()const
		{
			return m_uiRenderStateInheritFlag;
		}
		void SetRenderState(VSRenderState & RenderState);
		virtual uint32 SetBlendState(VSBlendState * pBlendState,bool bForceSet = false) = 0;
		virtual uint32 SetDepthStencilState(VSDepthStencilState * pDepthStencilState, bool bForceSet = false) = 0;
		virtual uint32 SetRasterizerState(VSRasterizerState * pRasterizerState, bool bForceSet = false) = 0;
		virtual void SetClipPlane(VSArray<Plane> &Plane,bool bEnable) = 0;
		virtual void SetScissorRect(VSArray<VSRect2> &Rect,bool bEnable) = 0;

		virtual void SetPTexAllState(VSTexAllState * pTexAllState,uint32 i);
		virtual void SetVTexAllState(VSTexAllState * pTexAllState,uint32 i);
		virtual void SetGTexAllState(VSTexAllState * pTexAllState, uint32 i);
		virtual void SetDTexAllState(VSTexAllState * pTexAllState, uint32 i);
		virtual void SetHTexAllState(VSTexAllState * pTexAllState, uint32 i);
		virtual void SetCTexAllState(VSTexAllState * pTexAllState, uint32 i);

		virtual uint32 SetVShader(VSVShader * pVShader);
		virtual uint32 SetPShader(VSPShader * pPShader);
		virtual uint32 SetGShader(VSGShader * pGShader);
		virtual uint32 SetDShader(VSDShader * pDShader);
		virtual uint32 SetHShader(VSHShader * pHShader);
		virtual uint32 SetCShader(VSCShader * pCShader);

		virtual uint32 SetVBuffer(VSVertexBuffer * pVBuffer, uint32 uiStream = 0) = 0;
		virtual uint32 SetIBuffer (VSIndexBuffer* pIBuffer) = 0;

		virtual uint32 BeginQuery(VSQuery * pQuery) = 0;
		virtual uint32 EndQuery(VSQuery * pQuery) = 0;

		virtual uint32 SetPTexture (VSTexture* pTexture,uint32 i) = 0;
		virtual uint32 SetVTexture (VSTexture* pTexture,uint32 i) = 0;
		virtual uint32 SetGTexture(VSTexture* pTexture, uint32 i) = 0;
		virtual uint32 SetDTexture(VSTexture* pTexture, uint32 i) = 0;
		virtual uint32 SetHTexture(VSTexture* pTexture, uint32 i) = 0;
		virtual uint32 SetCTexture(VSTexture* pTexture, uint32 i) = 0;
		virtual uint32 SetCBufferResource(VSBufferResource* pBufferResource, uint32 i) = 0;
		virtual uint32 SetVBufferResource(VSBufferResource* pBufferResource, uint32 i) = 0;
		virtual uint32 SetPBufferResource(VSBufferResource* pBufferResource, uint32 i) = 0;
		virtual uint32 SetGBufferResource(VSBufferResource* pBufferResource, uint32 i) = 0;
		virtual uint32 SetHBufferResource(VSBufferResource* pBufferResource, uint32 i) = 0;
		virtual uint32 SetDBufferResource(VSBufferResource* pBufferResource, uint32 i) = 0;

		virtual void Dispath(uint32 uiThreadGroupCountX, uint32 uiThreadGroupCountY,
			uint32 uiThreadGroupCountZ) = 0;

		virtual uint32 SetNormalMesh(VSGeometry *pGeometry);
		virtual uint32 SetInstanceMesh(VSInstanceGeometry *pInstanceGeometry);


		virtual uint32 SetPSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet = false) = 0;
		virtual uint32 SetVSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet = false) = 0;
		virtual uint32 SetGSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet = false) = 0;
		virtual uint32 SetDSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet = false) = 0;
		virtual uint32 SetHSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet = false) = 0;
		virtual uint32 SetCSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet = false) = 0;


		virtual bool SetRenderTargets(VSRenderTarget * pRenderTargets[], uint32 uiNum);
		virtual bool EndRenderTargets(VSRenderTarget * pRenderTargets[], uint32 uiNum);
		virtual bool SetRenderTarget(VSRenderTarget *pRenderTarget,uint32 i);
		virtual bool EndRenderTarget(VSRenderTarget *pRenderTarget,uint32 i);
		virtual bool SetDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer) = 0;
		virtual bool EndDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer) = 0;
		virtual bool SetCSUnorderAccess(VSUnorderAccess * pUnorderAccess, uint32 i);
		virtual bool EndCSUnorderAccess(VSUnorderAccess * pUnorderAccess, uint32 i);
		virtual bool SetCSUnorderAccesses(VSUnorderAccess * pUnorderAccesses[], uint32 uiNum);
		virtual bool EndCSUnorderAccesses(VSUnorderAccess * pUnorderAccesses[], uint32 uiNum);

		virtual uint32 GetTextureCompressLen(uint32 uiLen) = 0;

		void SetVSResourceSlotUsed(VSSlot * pSlotResource, uint32 uiSlot, bool b);
		void SetPSResourceSlotUsed(VSSlot * pSlotResource, uint32 uiSlot, bool b);
		void SetGSResourceSlotUsed(VSSlot * pSlotResource, uint32 uiSlot, bool b);
		void SetDSResourceSlotUsed(VSSlot * pSlotResource, uint32 uiSlot, bool b);
		void SetHSResourceSlotUsed(VSSlot * pSlotResource, uint32 uiSlot, bool b);
		void SetCSResourceSlotUsed(VSSlot * pSlotResource, uint32 uiSlot, bool b);

		void RenderStateToDefault();
		
		void EnableBindResourceUsed(VSSlot * pSlotResource);
		void DisableBindResourceUsed(VSSlot * pSlotResource);

		bool CheckIsResourceCanSet(VSSlot * pSlotResource);
		struct ChildWindowInfo
		{
			HWND	m_hHwnd;
			uint32 m_uiWidth;
			uint32 m_uiHeight;
			bool		 m_bDepth;
		};
		ChildWindowInfo * GetChildWindowInfo(int32 uiID);

		enum SupportFeatureType
		{
			SF_MulBufferSwtich,
			SF_GS,
			SF_MaterialValueInstance,
			SF_Tessellation,
			SF_MSBufferRead,
			SF_DepthStensilBufferRead,
			SF_DepthStensilToTexture,
			SF_BufferResource,
			SF_UnorderAccess,
			SF_CS,
			SF_ClearRange,
			SF_VertexIDInShader,
			SF_ShareSampler,
			SF_VShaderSampler,
			SF_AdvanceInstance,
			SF_NoPow2Texture,
			SF_MAX,
		};
		inline bool IsSupportFeature(SupportFeatureType SF) { return m_SupportFeature[SF]; }
protected:
		bool m_SupportFeature[SupportFeatureType::SF_MAX];

		HWND	m_hMainWindow;          // 主窗口						
		ChildWindowInfo * m_pChildWindowInfo;
		int		m_iNumChildWindow;          // 窗口个数
		int		m_iCurWindowID;
		bool	m_bIsRendering;


		VSColorRGBA m_ClearColor;
		float m_fClearDepth;
		uint32 m_uiClearStencil;

		VSFontRPtr			m_pFont;
		VSVertexFormatPtr   m_pQuadVertexFormat;
		VSVertexFormatPtr   m_pFontVertexFormat;
		VSVertexBuffer*		m_pVertexBuffer[STREAM_LEVEL];
		VSIndexBuffer*		m_pIndexBuffer;

		VSQuery * m_pQuery;

		VSArray<VSSlot *>m_pVTex;
		VSArray<VSSlot *>m_pPTex;
		VSArray<VSSlot *>m_pGTex;
		VSArray<VSSlot *>m_pDTex;
		VSArray<VSSlot *>m_pHTex;
		VSArray<VSSlot *>m_pCTex;

		VSVertexFormat*		m_pVertexFormat;
		VSVShader*			m_pVShader;
		VSPShader*			m_pPShader;
		VSGShader*			m_pGShader;
		VSDShader*			m_pDShader;
		VSHShader*			m_pHShader;
		VSCShader*			m_pCShader;
		
		VSMap<unsigned int,VSTexAllState * > m_pTexAllStateBuffer;
		VSMap <unsigned int, VSBufferResource * > m_pBufferResource;
		
		VSArray <VSSamplerState *>	m_pSamplerState[VSEngineFlag::ST_MAX];

		VSRenderState m_LocalRenderState;
		
		VSBlendState * m_pBlendState;
		VSDepthStencilState * m_pDepthStencilState;
		VSRasterizerState * m_pRasterizerState;
		bool				m_bClipPlaneEnable;
		bool				m_bScissorRectEnable;
		VSRenderState		m_UseState;
		unsigned int		m_uiRenderStateInheritFlag;

		enum //BACK_MACTH_TYPE
		{
			BMT_NONE = 0x00,
			BMT_RENDER_TARGET = 0x01,
		};
		unsigned int		m_uiBackMacthType;
		virtual void SetVShaderConstant(VSVShader * pShader) = 0;
		virtual void SetVShaderBindResource(VSVShader * pShader);

		virtual void SetGShaderConstant(VSGShader * pShader) = 0;
		virtual void SetGShaderBindResource(VSGShader * pShader);

		virtual void SetDShaderConstant(VSDShader * pShader) = 0;
		virtual void SetDShaderBindResource(VSDShader * pShader);

		virtual void SetHShaderConstant(VSHShader * pShader) = 0;
		virtual void SetHShaderBindResource(VSHShader * pShader);

		virtual void SetPShaderConstant(VSPShader * pShader) = 0;
		virtual void SetPShaderBindResource(VSPShader * pShader);

		virtual void SetCShaderConstant(VSCShader * pShader) = 0;
		virtual void SetCShaderBindResource(VSCShader * pShader);


		virtual bool SetVShaderProgram(VSVShader*	pVShaderProgram) = 0;
		virtual bool SetPShaderProgram(VSPShader*	pPShaderProgram) = 0;
		virtual bool SetGShaderProgram(VSGShader*	pGShaderProgram) = 0;
		virtual bool SetDShaderProgram(VSDShader*	pDShaderProgram) = 0;
		virtual bool SetHShaderProgram(VSHShader*	pHShaderProgram) = 0;
		virtual bool SetCShaderProgram(VSCShader*	pCShaderProgram) = 0;
		//Load Release
		virtual bool OnLoadVShaderProgram (VSVShader * pVShaderProgram,VSResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseResouce(VSResourceIdentifier * pResourceResourceIdentifier);

		virtual bool OnLoadPShaderProgram (VSPShader* pPShaderProgram,VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadGShaderProgram(VSGShader * pGShaderProgram, VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadDShaderProgram(VSDShader * pDShaderProgram, VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadHShaderProgram(VSHShader * pHShaderProgram, VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadCShaderProgram(VSCShader * pCShaderProgram, VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadTexture (VSTexture * pTexture,VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadDepthStencil (VSDepthStencil * pDepthStencil,VSResourceIdentifier *&pID) = 0;
		
		virtual bool OnLoadRenderTarget (VSRenderTarget * pRenderTarget,VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadUnorderAccess(VSBufferUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID) = 0;
		virtual bool OnLoadUnorderAccess(VSTextureUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadVBufferData (VSVertexBuffer * pVBuffer,VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadVBufferFormat (VSVertexFormat * pVertexFormat,VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadIBuffer (VSIndexBuffer* pIBuffer,VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadSBuffer(VSBufferResource* pSBuffer, VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadBlendState (VSBlendState * pBlendState,VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadDepthStencilState (VSDepthStencilState * pDepthStencilState,VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadRasterizerState(VSRasterizerState * pRasterizerState,VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadSamplerState(VSSamplerState * pSamplerState,VSResourceIdentifier *&pID) = 0;

		virtual bool OnLoadQuery(VSQuery * pQuery, VSResourceIdentifier *&pID) = 0;

		virtual uint32 SetVertexFormat(VSVertexFormat * pVertexFormat) = 0;

		virtual void * Lock(VSVertexBuffer * pVertexBuffer) = 0;
		virtual void UnLock(VSVertexBuffer * pVertexBuffer) = 0;

		virtual void * Lock(VSIndexBuffer * pIndexBuffer) = 0;
		virtual void UnLock(VSIndexBuffer * pIndexBuffer) = 0;

		virtual void * Lock(VSBufferResource * pStructBuffer) = 0;
		virtual void UnLock(VSBufferResource * pStructBuffer) = 0;

		virtual void * Lock(VSTexture * pTexture,uint32 uiLevel,uint32 uiFace) = 0;
		virtual void UnLock(VSTexture * pTexture,uint32 uiLevel,uint32 uiFace) = 0;

		virtual void * Lock(VSRenderTarget * pRenderTarget) = 0;
		virtual void UnLock(VSRenderTarget * pRenderTarget) = 0;

		virtual void * Lock(VSUnorderAccess  * pUnorderAccess) = 0;
		virtual void UnLock(VSUnorderAccess  * pUnorderAccess) = 0;

		virtual void * Lock(VSDepthStencil * pDepthStencil) = 0;
		virtual void UnLock(VSDepthStencil * pDepthStencil) = 0;

		virtual bool GetData(VSQuery * pQuery,void * pData, uint32 uiSize) = 0;

		virtual bool CopyResourceBuffer(VS2DTexture * pSource,VSCubeTexture * pDest,uint32 uiFace) = 0;
		
		bool SetDefaultValue();
		bool ReleaseDefaultValue();

		virtual bool ReleaseBindObjectResource();
	protected:
		enum	//device type
		{
			DT_HAL = 0,
			DT_REF = 1,
			DT_MAX
		};
		
		unsigned int	m_uinAdapter;
		unsigned int	m_uiDevType;
		VSString		m_AdapterName;
		UINT		m_uiScreenWidth;
		UINT		m_uiScreenHeight;
		UINT		m_uiCurRTWidth;
		UINT		m_uiCurRTHeight;
		UINT		m_uiBufferFormat;
		bool		m_bWindowed;


		UINT		m_uiDepthStencilFormat;
		UINT		m_uiCurAnisotropy;
		UINT		m_uiCurMultisample;
		UINT		m_uiCurRTMultisample;
		DWORD		m_dwMultisampleQuality;
		UINT		m_uiMaxMultisample;		
		UINT		m_uiMaxTextureWidth;
		UINT		m_uiMaxTextureHeight;

		UINT		m_uiMaxUseClipPlane;

		UINT		m_uiMaxBindResource[VSEngineFlag::ST_MAX];


		UINT		m_uiMaxSampler[VSEngineFlag::ST_MAX];


		UINT		m_uiMaxAnisotropy;

		UINT		m_uiMaxRTNum;
		UINT		m_uiMaxUAVNum;
	

		static VSString ms_ShaderProgramMain[VSEngineFlag::ST_MAX];


		static uint32 ms_uiBytesPerPixel[SFT_MAX];
		static uint32 ms_uiMinDimension[SFT_MAX];
		static uint32 ms_uiChannelPerPixel[SFT_MAX];
		static SCREEN_QUAD_TYPE ms_FullScreen[4];
		static MATH_USHORT_INDEX ms_FullScreenI[6];
		VSArray<SCREEN_FONT_TYPE> m_FontVertex;
		VSArray<MATH_USHORT_INDEX> m_FontIndex;

		VSViewPort m_CurViewPort;

		static VSString ms_ShaderTypeString[VSRenderer::RAT_MAX];
		static VSString ms_ShaderTypePathString[VSRenderer::RAT_MAX];
	//shader string
	public:
		//shader function
		virtual VSString TexColorBuffer(const VSColorBuffer * pColorBuffer) const = 0;
		virtual VSString Tex2D(const VS2DTexSampler * p2DTexSampler,uint32 uiShaderType) const= 0;
		virtual VSString Tex1D(const VS1DTexSampler * p1DTexSampler, uint32 uiShaderType) const= 0;
		virtual VSString Mul(const VSMul * pMul) const= 0;
		virtual VSString Div(const VSDiv * pDiv) const = 0;
		virtual VSString Dot(const VSDot * pDot) const= 0;
		virtual VSString Length(const VSLength * pLength) const = 0;
		virtual VSString Sub(const VSSub * pSub) const= 0;
		virtual VSString Add(const VSAdd * pAdd) const = 0;
		virtual VSString MakeValue(const VSMakeValue * pMakeValue) const = 0;
		virtual VSString Saturate(const VSSaturate * pSaturate) const = 0;
		//shader key word
		enum //Float Index
		{
			FI_1,
			FI_2,
			FI_3,
			FI_4,
			FI_MAX

		};
		virtual VSString FloatByChannelNum(uint32 ChannelNum)const = 0;
		VSString Float(uint32 uiIndex)const;
		virtual VSString Float()const =  0;
		virtual VSString Float2()const =  0;
		virtual VSString Float3()const =  0;
		virtual VSString Float4()const =  0;
		virtual VSString Return()const = 0;

		virtual VSString FloatConst(const VSString & Value1)const =  0;
		virtual VSString Float2Const(const VSString & Value1,const VSString & Value2)const =  0;
		virtual VSString Float3Const(const VSString & Value1,
						const VSString & Value2,const VSString & Value3)const =  0;
		virtual VSString Float4Const(const VSString & Value1,const VSString & Value2,
					const VSString & Value3,const VSString & Value4)const =  0;

		virtual void CustomPointLightUseString(VSString & PointLightUseString ,VSString & PointAttenuationDivString) = 0;

		virtual void CustomSpotLightUseString(VSString & SpotLightUseString ,VSString & SpotAttenuationDivString) = 0;

		enum //Value Element
		{
			VE_NONE = 0,
			VE_A = BIT(0),
			VE_R = BIT(1),
			VE_G = BIT(2),
			VE_B = BIT(3),
			DF_ALL = 0X0F
		};
		static VSString GetValueElement(const VSPutNode * pPutNode,
			unsigned char uiVE);
		static VSString GetValueElement(const VSString & InputString,
			unsigned char uiVE);
		
		virtual void GetShareSamplerDeclare(class VSShader * pShader, VSString & OutString);

		virtual VSString GetDeclareValue(const VSString &ShowName, uint32 uiValueType, uint32 uiRegisterIndex)const = 0;		
		virtual VSString GetDeclareSampler(const VSString &ShowName, uint32 SamplerType, uint32 uiRegisterIndex, uint32 SamplerSource)const = 0;
		virtual void GetDeclareShareSampler(uint32 SamplerSource, uint32 uiRegisterIndex, VSString & OutString)const = 0;
		//shader custom function
		virtual void CreateVInputDeclare(MaterialShaderPara &MSPara,
										VSString & OutString) = 0;
		virtual void CreateVOutputDeclare(MaterialShaderPara &MSPara,
										VSString & OutString) = 0;
		virtual void CreateVUserConstant(VSVShader * pVShader,MaterialShaderPara &MSPara,
										VSString & OutString) = 0;
		virtual void CreateVFunction(MaterialShaderPara &MSPara,
										VSString & OutString) = 0;
		
		virtual void SetMaterialVShaderConstant(MaterialShaderPara &MSPara,VSVShader *pVShader) = 0;

		virtual void CreatePInputDeclare(MaterialShaderPara &MSPara,
										VSString & OutString) = 0;
		virtual void CreatePOutputDeclare(MaterialShaderPara &MSPara,
										VSString & OutString) = 0;
		virtual void CreatePUserConstant(VSPShader * pPShader,MaterialShaderPara &MSPara,
										VSString & OutString) = 0;
		virtual void CreatePFunction(MaterialShaderPara &MSPara,
										VSString & OutString) = 0;

		virtual void SetMaterialPShaderConstant(MaterialShaderPara &MSPara,VSPShader *pPShader) = 0;

		virtual void CreateGInputDeclare(MaterialShaderPara &MSPara, 
			VSString & OutString) = 0;
		virtual void CreateGOutputDeclare(MaterialShaderPara &MSPara, 
			VSString & OutString) = 0;
		virtual void CreateGUserConstant(VSGShader * pGShader, MaterialShaderPara &MSPara,
			VSString & OutString) = 0;
		virtual void CreateGFunction(MaterialShaderPara &MSPara,
			VSString & OutString) = 0;
		virtual void SetMaterialGShaderConstant(MaterialShaderPara &MSPara,VSGShader *pGShader) = 0;

		virtual void CreateHInputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString) = 0;
		virtual void CreateHOutputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString) = 0;
		virtual void CreateHUserConstant(VSHShader * pHShader, MaterialShaderPara &MSPara,
			VSString & OutString) = 0;
		virtual void CreateHFunction(MaterialShaderPara &MSPara,
			VSString & OutString) = 0;
		virtual void SetMaterialHShaderConstant(MaterialShaderPara &MSPara, VSHShader * pHShader) = 0;

		virtual void CreateDInputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString) = 0;
		virtual void CreateDOutputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString) = 0;
		virtual void CreateDUserConstant(VSDShader * pDShader, MaterialShaderPara &MSPara,
			VSString & OutString) = 0;
		virtual void CreateDFunction(MaterialShaderPara &MSPara,
			VSString & OutString) = 0;
		virtual void SetMaterialDShaderConstant(MaterialShaderPara &MSPara, VSDShader *pDShader) = 0;

		virtual void GetIncludeShader(VSString &OutString) = 0;
		
		virtual void GetDynamicShader(VSString &OutString) = 0;

		virtual void SkyLight(const VSString &WorldNormal,const VSString &UpColor,const VSString & DownColor, const VSString & DiffuseColor,VSString & OutString) = 0;
		virtual void DirectionalLight(int32 iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
									const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
									VSString & OutString)const = 0;
		virtual void CustomDirectionalLight(int32 iLightNum,const VSString &CustomString,VSArray<VSString> ShadowString,VSString & OutString)const = 0;

		virtual void OrenNayarDirectionalLight(int32 iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
									const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
									VSString & OutString)const = 0;
	

		virtual void PointLight(int32 iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
							const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
							VSString & OutString)const = 0;
		virtual void CustomPointLight(int32 iLightNum,const VSString &CustomString,const VSString & WorldPos,VSArray<VSString> ShadowString,VSString & OutString)const = 0;

		virtual void OrenNayarPointLight(int32 iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
							const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
							VSString & OutString)const = 0;
		

		virtual void SpotLight(int32 iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
								VSString & OutString)const = 0;
		virtual void CustomSpotLight(int32 iLightNum,const VSString &CustomString,const VSString & WorldPos,VSArray<VSString> ShadowString,VSString & OutString)const = 0;
		virtual void OrenNayarSpotLight(int32 iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
								VSString & OutString)const = 0;

		virtual void LocalToWorldPos(const VSString & LocalPos,VSString &OutString)const = 0;
		virtual void WorldToViewPos(const VSString & LocalPos,VSString &OutString)const = 0;

		virtual void LocalToWorldNormal(const VSString & LocalNormal,VSString &OutString)const = 0;

		virtual void WorldToViewNormal(const VSString & WorldNormal,VSString &OutString)const = 0;

		virtual void GetWorldViewDir(VSString &OutString)const = 0;

		virtual void TransProjPos(const VSString & Pos,const VSString & Matrix,VSString &OutString)const = 0;
		virtual void BumpNormal(const VSString &TexNormal,VSString &OutString)const = 0;

		virtual void ComputeDir(const VSString & Orgin,const VSString & End,VSString &OutString)const = 0;
		virtual void ComputeLength(const VSString & Orgin,const VSString & End,VSString &OutString)const = 0;
		virtual void ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight,const VSString &BoneVector,
					 const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const = 0;
		virtual void ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight,
					const VSString &AnimDataResource, const VSString & AnimAllInfo,
			const VSString &AnimInstanceInfo, const VSString &AnimNum,
					const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const = 0;

		virtual void BoneTranPos( const VSString & LocalPos,const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const = 0;
		virtual void BoneTranNormal( const VSString & LocalNormal,const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const = 0;

		virtual void DecodeNormal1(const VSString & Normal,VSString &OutString) const= 0;
		virtual void RecodeNormal1(const VSString & Normal,VSString &OutString) const = 0;
		virtual void DecodeNormal2(const VSString & Normal,const VSString & Tengent,VSString &OutString) const= 0;

		virtual void DecodeNormal3(const VSString & Normal,VSString &OutString) const = 0;
		virtual void RecodeNormal3(const VSString & Normal,VSString &OutString) const = 0;

		virtual void Reciprocal(const VSString & Value, VSString &OutString)const = 0;
		virtual void Clip(const VSString & Value,VSString &OutString)const = 0;
		virtual void GreaterZeroPow(const VSString & Value,float Exp,VSString &OutString) const= 0;
		virtual void SaturatePow(const VSString & Value,float Exp,VSString &OutString) const= 0;
		virtual void Saturate(const VSString & Value,VSString &OutString)const = 0;

		virtual void PointLightCubeShadow(const VSString & PointLightName , const VSString & CubeShadowSampler , const VSString & ViewMatrix,
									const VSString & WorldPos, VSString& OutString)const = 0;

		virtual void PointLightCubeShadow(const VSString & PointLightName , const VSString & CubeShadowSampler ,
			const VSString & WorldPos, VSString& OutString)const = 0;

		virtual void PointLightVolumeShadow(const VSString & PointLightName , const VSString & VolumeShadowSampler , 
									const VSString & WorldPos, VSString& OutString)const = 0;

		virtual void PointLightDualParaboloidShadow(const VSString & PointLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const = 0;
		virtual void DirectionLightVolumeShadow(const VSString & DirLightName , const VSString & VolumeShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const = 0;

		virtual void DirectionLightCSMShadow(const VSString & DirLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, const VSString & ViewPos,const VSString &FarZ,VSString& OutString)const = 0;

		virtual void DirectionLightShadow(const VSString & DirLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const = 0;

		virtual void SpotLightShadow(const VSString & SpotLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const = 0;

		virtual void TranLightToTemp(VSArray<VSLight *> LightArray,VSString & OutString)const = 0;

		virtual void GetLightFunction(VSArray<VSLight *> LightArray,const VSString & WorldPos,VSString & OutString)const = 0;

		virtual void EncodeReflect(const VSString & Mip , const VSString & Pow , VSString& OutString) = 0;

		virtual void DecodeReflect(const VSString & Mip , const VSString & Pow , const VSString & Value,VSString& OutString) = 0;

	public:
		static VSRenderer * ms_pRenderer;
		static const VSString & GetRenderTypeShaderPath(uint32 RenderTypeAPI);
		static const VSString & GetRenderTypeString(uint32 RenderTypeAPI);
		const VSString & GetRenderTypeShaderPath()const;
		const VSString & GetRenderTypeString()const;
	};
}


