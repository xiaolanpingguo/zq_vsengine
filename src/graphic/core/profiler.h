#pragma once


//#include "VSString.h"
//#include "datastruct/VSArray.h"
#include "graphic/core/object.h"


namespace zq
{
	class VSRenderer;
	class VSProfilerNode;
	struct ProfilerString
	{
		int32 iX;
		int32 iY;
		VSColorRGBA rColor;
		VSString sText;
	};
	class GRAPHIC_API VSProfiler
	{
	public:
		DECLARE_INITIAL_ONLY
		static void AddProfileNode(VSProfilerNode * pProfilerNode);
		static void Draw();
		static void Draw(VSRenderer * pRenderer);
		static void GetProfilerNameString(const VSString & Name, uint32 uiLayer,VSString & OutString);
		static void ClearAll();
		static uint32 ms_uiCurCount;
	protected:
		static VSArray<VSProfilerNode *> & GetProfilerNode()
		{
			static VSArray<VSProfilerNode *> m_pProfilerNodes;
			return m_pProfilerNodes;
		}
		
		static bool InitialDefaultState();
		static VSArray<VSProfilerNode *> m_pRoot;
		static char * ms_LayerFlag[10];
	};

	class GRAPHIC_API VSProfilerNode
	{
	public:
		VSProfilerNode(const char *Name = NULL, const char * ParentName = NULL);
		
		virtual void Clear() = 0;
		void AddChild(VSProfilerNode * pChild);
		void Draw( uint32 uiLayer);
	protected:
		virtual void OnDraw( uint32 uiLayer) = 0;
		friend class VSProfilerNodeTImer;
		friend class VSProfiler;
		
		VSProfilerNode * m_pParentNode;
		VSArray<VSProfilerNode*>m_pChildNode;
		VSString m_Name;
		VSString m_ParentName;
	};
	class GRAPHIC_API VSTimeProfilerNode : public VSProfilerNode
	{
	public:
		class GRAPHIC_API VSProfilerNodeTImer
		{
		public:
			VSProfilerNodeTImer(VSTimeProfilerNode * pProfilerNode);
			~VSProfilerNodeTImer();
		protected:
			VSTimeProfilerNode * m_pOwner;
			float m_fBeginTime;
		};
		VSTimeProfilerNode(const char *Name = NULL, const char * ParentName = NULL);
		virtual void Clear()
		{
			m_fProfilerTime = 0.0f;
		}
		virtual void OnDraw(uint32 uiLayer);
	protected:
		float m_fProfilerTime;
	};
	class GRAPHIC_API VSCountProfilerNode : public VSProfilerNode
	{
	public:
		class GRAPHIC_API VSProfilerNodeCounter
		{
		public:
			VSProfilerNodeCounter(VSCountProfilerNode * pProfilerNode, int32 uiCount);
		};
		VSCountProfilerNode(const char *Name = NULL, const char * ParentName = NULL);
		virtual void Clear()
		{
			m_uiCounter = 0;
		}
		virtual void OnDraw( uint32 uiLayer);
	protected:
		int32 m_uiCounter;
	};
	class GRAPHIC_API VSNoClearCountProfilerNode : public VSCountProfilerNode
	{
	public:
		VSNoClearCountProfilerNode(const char *Name = NULL, const char * ParentName = NULL);
		virtual void Clear()
		{
		}
	};
	class GRAPHIC_API VSOnlyTimeProfilerNode : public VSProfilerNode
	{
	public:
		class GRAPHIC_API VSProfilerNodeOnlyTImer
		{
		public:
			VSProfilerNodeOnlyTImer(VSOnlyTimeProfilerNode * pProfilerNode, float fProfilerTime);
		};
		VSOnlyTimeProfilerNode(const char *Name = NULL, const char * ParentName = NULL);
		virtual void Clear()
		{
			m_fProfilerTime = 0.0f;
		}
		virtual void OnDraw( uint32 uiLayer);
	protected:
		float m_fProfilerTime;
	};
#ifdef PROFILER
#define EXTERN_TIME_PROFILENODE(Name) extern GRAPHIC_API VSTimeProfilerNode TimeProfilerNode_##Name;
#define DECLEAR_TIME_PROFILENODE(Name,ParentName)  VSTimeProfilerNode TimeProfilerNode_##Name(_T(#Name),_T(#ParentName));
#define ADD_TIME_PROFILE(Name) VSTimeProfilerNode::VSProfilerNodeTImer ProfilerNodeTimer_##Name(&TimeProfilerNode_##Name);

#define EXTERN_COUNT_PROFILENODE(Name) extern GRAPHIC_API VSCountProfilerNode  CountProfilerNode_##Name;
#define DECLEAR_COUNT_PROFILENODE(Name,ParentName)  VSCountProfilerNode CountProfilerNode_##Name(_T(#Name),_T(#ParentName));
#define EXTERN_NOCLEAR_COUNT_PROFILENODE(Name) extern GRAPHIC_API VSNoClearCountProfilerNode  CountProfilerNode_##Name;
#define DECLEAR_NOCLEAR_COUNT_PROFILENODE(Name,ParentName)  VSNoClearCountProfilerNode CountProfilerNode_##Name(_T(#Name),_T(#ParentName));
#define ADD_COUNT_PROFILE(Name,Count) VSCountProfilerNode::VSProfilerNodeCounter ProfilerNodeCounter_##Name(&CountProfilerNode_##Name,Count);

#define EXTERN_ONLYTIME_PROFILENODE(Name) extern GRAPHIC_API VSOnlyTimeProfilerNode  OnlyTimeProfilerNode_##Name;
#define DECLEAR_ONLYTIME_PROFILENODE(Name,ParentName)  VSOnlyTimeProfilerNode OnlyTimeProfilerNode_##Name(_T(#Name),_T(#ParentName));
#define ADD_ONLYTIME_PROFILE(Name,fProfilerTime) VSOnlyTimeProfilerNode::VSProfilerNodeOnlyTImer ProfilerNodeOnlyTimer_##Name(&OnlyTimeProfilerNode_##Name,fProfilerTime);
#else
#define EXTERN_TIME_PROFILENODE(Name)
#define DECLEAR_TIME_PROFILENODE(Name,ParentName)  
#define ADD_TIME_PROFILE(Name) 

#define EXTERN_COUNT_PROFILENODE(Name)
#define EXTERN_NOCLEAR_COUNT_PROFILENODE(Name)
#define DECLEAR_COUNT_PROFILENODE(Name,ParentName)  
#define DECLEAR_NOCLEAR_COUNT_PROFILENODE(Name,ParentName)  
#define ADD_COUNT_PROFILE(Name,Count) 

#define EXTERN_ONLYTIME_PROFILENODE(Name)
#define DECLEAR_ONLYTIME_PROFILENODE(Name,ParentName)  
#define ADD_ONLYTIME_PROFILE(Name,fProfilerTime) 
#endif
}