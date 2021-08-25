#pragma once


#include "graphic/render/renderstate/depthstencilstate.h"
#include "graphic/render/renderstate/blendstate.h"
#include "graphic/render/renderstate/rasterizerstate.h"
#include "math/plane.h"
#include "math/rect2.h"
#include "datastruct/VSArray.h"


namespace zq
{
	class VSStream;
	class GRAPHIC_API VSRenderState : public VSObject
	{
		friend class VSSpatial;
		friend class VSRenderer;
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //Inherit Flag
		{
			IF_WIRE_ENABLE = 0X0000001,
			IF_CULL_TYPE = 0X0000002,

			IF_ALL = 0XFFFFFFFF
		};
		VSRenderState()
		{
			m_pBlendState = (VSBlendState *)VSBlendState::GetDefault();
			m_pDepthStencilState = (VSDepthStencilState *)VSDepthStencilState::GetDefault();
			m_pRasterizerState = (VSRasterizerState *)VSRasterizerState::GetDefault();
			m_RectArray.Clear();
			m_Plane.Clear();
		}
		~VSRenderState()
		{
			m_pBlendState = NULL;
			m_pDepthStencilState = NULL;
			m_pRasterizerState = NULL;
		}

		const VSRenderState & operator =(const VSRenderState &RenderState)
		{
			GetAll(&RenderState);
			return *this;
		}
		const VSRenderState & operator =(const VSRenderState *RenderState)
		{
			GetAll(RenderState);
			return *this;
		}
		void GetAll(const VSRenderState * pRenderState)
		{

			VSMAC_ASSERT(pRenderState);
			if (pRenderState)
			{
				SetRasterizerState(pRenderState->m_pRasterizerState);
				SetDepthStencilState(pRenderState->m_pDepthStencilState);
				SetBlendState(pRenderState->m_pBlendState);
				m_RectArray = pRenderState->m_RectArray;
				m_Plane = pRenderState->m_Plane;
			}
			else
			{
				ClearState();
			}
		}
		void Inherit(const VSRenderState * pRenderState,uint32 uiInheritFlag = 0);
		void SwapCull();
		
		void AddRect(const VSRect2 & Rect)
		{
			m_RectArray.AddElement(Rect);
		}
		void ClearRect()
		{
			m_RectArray.Clear();
		}

		void AddPlane(const Plane & Plane)
		{
			m_Plane.AddElement(Plane);
		}
		void ClearPlane()
		{
			m_Plane.Clear();
		}

		void ClearState()
		{	
			m_pDepthStencilState = NULL;
			m_pBlendState = NULL;
			m_pRasterizerState = NULL;
			m_RectArray.Clear();
			m_Plane.Clear();
		}
		void SetBlendState(VSBlendState * pBlendState)
		{
			if (!pBlendState)
			{
				pBlendState = (VSBlendState *)VSBlendState::GetDefault();
			}
			if (m_pBlendState != pBlendState)
			{
				m_pBlendState = pBlendState;
				m_BlendDesc = pBlendState->GetBlendDesc();
			}

		}
		void SetDepthStencilState(VSDepthStencilState *pDepthStencilState)
		{
			if (!pDepthStencilState)
			{
				pDepthStencilState = (VSDepthStencilState *)VSDepthStencilState::GetDefault();
			}
			if (m_pDepthStencilState != pDepthStencilState)
			{
				m_pDepthStencilState = pDepthStencilState;
				m_DepthStencilDesc = pDepthStencilState->GetDepthStencilDesc();
			}
		}
		void SetRasterizerState(VSRasterizerState *pRasterizerState)
		{
			if (!pRasterizerState)
			{
				pRasterizerState = (VSRasterizerState *)VSRasterizerState::GetDefault();
			}
			if (m_pRasterizerState != pRasterizerState)
			{
				m_pRasterizerState = pRasterizerState;
				m_RasterizerDesc = pRasterizerState->GetRasterizerDesc();
			}
		}
		const VSDepthStencilState * GetDepthStencilState()const
		{
			return m_pDepthStencilState;
		}
		const VSBlendState * GetBlendState()const
		{
			return m_pBlendState;
		}
		const VSRasterizerState * GetRasterizerState()const
		{
			return m_pRasterizerState;
		}
		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
	protected:
		VSDepthStencilStatePtr	m_pDepthStencilState;
		VSBlendStatePtr			m_pBlendState;
		VSRasterizerStatePtr	m_pRasterizerState;

		VSDepthStencilDesc m_DepthStencilDesc;
		VSRasterizerDesc m_RasterizerDesc;
		VSBlendDesc m_BlendDesc;

		VSArray<VSRect2> m_RectArray;
		//Clip Space Plane : the distance to plane > 0 will be discard.
		VSArray<Plane> m_Plane;
	};
	VSTYPE_MARCO(VSRenderState);
	DECLARE_Ptr(VSRenderState);
}