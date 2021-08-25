#pragma once
#include "VSList.h"
#include "VSMap.h"
#include "VSStack.h"
#include "math/maths.h"
namespace zq
{
	class VSGraphEdge;
	class VSDATESTRUCT_API VSGraphNode 
	{
	public:
		VSGraphNode()
		{
			m_Edge.Clear();
		}
		~VSGraphNode()
		{

		}
		void AddEdge(VSGraphEdge * pEdge);

		VSList<VSGraphEdge *> &GetEdge()
		{
			return m_Edge;
		}
	protected:
		//��ͨ���ı�
		VSList<VSGraphEdge *> m_Edge;
	};
	class VSDATESTRUCT_API VSGraphEdge
	{
	public:
		VSGraphEdge()
		{
			m_pFrom = NULL;
			m_pTo = NULL;
		}
		~VSGraphEdge()
		{
			m_pFrom = NULL;
			m_pTo = NULL;
		}
		inline void SetFrom(VSGraphNode * pGraphNode)
		{
			VSMAC_ASSERT(pGraphNode);
			m_pFrom = pGraphNode;	
		}
		inline void SetTo(VSGraphNode *pGraphNode)
		{
			VSMAC_ASSERT(pGraphNode);
			m_pTo = pGraphNode;
		}
		inline void SetCost(float fCost)
		{
			m_fCost = fCost;
		}

		inline VSGraphNode * GetFrom()
		{
			return m_pFrom;
		}
		inline VSGraphNode * GetTo()
		{
			return m_pTo;
		}
		inline float GetCost()
		{
			return m_fCost;
		}
	protected:
		VSGraphNode * m_pFrom;
		VSGraphNode * m_pTo;
		float m_fCost;
	};

	class VSDATESTRUCT_API VSGraph
	{
	public:
		VSGraph(bool bIsDig = false);
		~VSGraph();
		
		virtual VSGraphNode * CreateNode();
		void DeleteNode(VSGraphNode * pNode);
		virtual VSGraphEdge *CreateEdge(VSGraphNode * pFrom,VSGraphNode * pTo,float fCost = 0.0f);
		void DeleteEdge(VSGraphEdge * pEdge);
		void DeleteEdge(VSGraphNode * pFrom,VSGraphNode * pTo);
		unsigned int GetNodeNum()const;
		unsigned int GetEdgeNum()const;
		VSList<VSGraphNode *> & GetAllNode()
		{
			return m_NodeList;
		}
		VSList<VSGraphEdge *> &GetAllEdge()
		{
			return m_EdgeList;
		}
	protected:
		bool m_bIsDig;
		VSList<VSGraphNode *> m_NodeList;
		VSList<VSGraphEdge *> m_EdgeList;
	};
	
	
	
	template<class T,unsigned int uiNodeNum>
	T *CreateFromAdjacencyMatrix(float pMatrixArray[uiNodeNum][uiNodeNum])
	{
		if (!pMatrixArray || !uiNodeNum)
		{
			return NULL;
		}
		VSGraph * pGraph = MEM_NEW T(true);
		VSGraphNode ** pGraphNode = MEM_NEW VSGraphNode *[uiNodeNum];

		for (unsigned int i = 0 ; i < uiNodeNum ; i++)
		{
			pGraphNode[i] = pGraph->CreateNode();

		}
		for (unsigned int i = 0 ; i < uiNodeNum ; i++)
		{
			for (unsigned int j = 0 ; j < uiNodeNum ; j++)
			{
				if (i == j)
				{
					continue;
				}
				if(pMatrixArray[i][j] > 0.0f)
				{
					pGraph->CreateEdge(pGraphNode[i],pGraphNode[j],pMatrixArray[i][j]);
				}
			}
		}
		SAFE_DELETEA(pGraphNode);
		return pGraph;
	}
	//0 ����ͨ�� ����0��ʾͨ������
	template<class T>
	T *CreateFrom2DMatrix(unsigned char ** pMatrixArray,int uiWidth,int uiHeight)
	{
		if (!pMatrixArray || !uiWidth ||!uiHeight)
		{
			return NULL;
		}
		VSGraph * pGraph = MEM_NEW T(false);
		VSGraphNode ** pGraphNode = MEM_NEW VSGraphNode *[uiWidth * uiHeight];
		for (int i = 0 ; i < uiHeight ; i++)
		{
			for (int j = 0 ; j < uiWidth ;j++)
			{
				pGraphNode[i * uiWidth + j] = pGraph->CreateNode();
			}
			
		}
		struct TempElement
		{
			TempElement()
			{

			}
			TempElement(int i,int j)
			{
				di = i;
				dj = j;
			}
			~TempElement()
			{
			}
			int di;
			int dj;
		};
		VSArray<TempElement> EArray;
		EArray.AddElement(TempElement(-1,-1));
		EArray.AddElement(TempElement(-1,0));
		EArray.AddElement(TempElement(-1,1));
		EArray.AddElement(TempElement(0,-1));
		EArray.AddElement(TempElement(0,1));
		EArray.AddElement(TempElement(1,-1));
		EArray.AddElement(TempElement(1,0));
		EArray.AddElement(TempElement(1,1));
		for (int i = 0 ; i < uiHeight ; i++)
		{
			for (int j = 0 ; j < uiWidth ; j++)
			{
				if (pGraphNode[i][j] > 0 && i != j)
				{
					for (unsigned int k = 0 ; k < EArray.GetNum() ; k++)
					{
						int Usei = EArray[k].di + i;
						if ( Usei < 0 || Usei >= uiHeight) 
						{
							continue;
						}
						int Usej = EArray[k].dj + j;
						if (Usej < 0 || Usej >= uiWidth) 
						{
							continue;
						}
						if (pGraphNode[Usei][Usej] > 0)
						{
							pGraph->CreateEdge(pGraphNode[i][j],pGraphNode[Usei][Usej],1.0f);
						}

					}
				}
				
				
					
				
			}
		}
		SAFE_DELETEA(pGraphNode);
		return pGraph;

	}

	class VSDATESTRUCT_API VSGraph_Search
	{
	protected:
		VSGraphNode * m_pSource;
		VSGraphNode * m_pTarget;
		virtual void Search() = 0;
		VSGraph * m_pGraph;
	public:
		VSGraph_Search( VSGraph * pGraph,VSGraphNode * pSource,VSGraphNode * pTarget);
		virtual ~VSGraph_Search();
		virtual VSArray<VSGraphNode *> GetPathToTarget()const = 0;
		virtual float GetCostToTarget()const = 0;
	};
	
	
	class VSDATESTRUCT_API VSGraph_SearchCommon : public VSGraph_Search
	{
	public:
		VSGraph_SearchCommon( VSGraph * pGraph,VSGraphNode * pSource,VSGraphNode * pTarget);
		virtual ~VSGraph_SearchCommon();
		virtual VSArray<VSGraphNode *> GetPathToTarget()const;
	protected:
		unsigned int RemoveEdge(VSGraphNode * pGraphNode);
		void ReSort(VSGraphNode * pGraphNode,float fCost);
		VSMapOrder<VSGraphNode * , VSGraphEdge *> m_NodePathShortestEdge;
		VSMapOrder<VSGraphNode * ,VSGraphEdge *> m_NodeCurShortEdge;
		struct SortElement
		{
		public:
			SortElement()
			{
				m_fCost = 0.0f;
				m_pGraphNode = NULL;
			}
			~SortElement()
			{	

			}
			float m_fCost;
			VSGraphNode * m_pGraphNode;
			bool operator > (const SortElement& e)const
			{
				return m_fCost < e.m_fCost;
			}
			bool operator < (const SortElement& e)const
			{
				return m_fCost > e.m_fCost;
			}
			bool operator == (const SortElement& e)const
			{
				return m_fCost == e.m_fCost;
			}
		};

		VSArrayOrder<SortElement> m_Priority;
	};
	class VSDATESTRUCT_API VSGraph_SearchDijkstra : public VSGraph_SearchCommon
	{
	public:
		VSGraph_SearchDijkstra(VSGraph * pGraph,VSGraphNode * pSource,VSGraphNode * pTarget);
		virtual ~VSGraph_SearchDijkstra();
		
		virtual float GetCostToTarget()const;
		virtual float GetCostToNode(VSGraphNode * pGraphNode)const;
	protected:
		virtual void Search();
		
		VSMapOrder<VSGraphNode * , float> m_CostToThisNode;
		
		
	};
	class VSDATESTRUCT_API VSHeuristic
	{
	public:
		static float Calculate(VSGraphNode * pN1,VSGraphNode * pN2)
		{
			return 0.0f;
		}
	};
	template<class VSHeuristic>
	class VSGraph_SearchAStar : public VSGraph_SearchCommon
	{
	public:
		VSGraph_SearchAStar(VSGraph * pGraph,VSGraphNode * pSource,VSGraphNode * pTarget);
		virtual ~VSGraph_SearchAStar();
		virtual float GetCostToTarget()const;
	protected:
		virtual void Search();
		VSMapOrder<VSGraphNode * , float> m_CostFToThisNode;
		VSMapOrder<VSGraphNode * , float> m_CostGToThisNode;
	};
	template<class VSHeuristic>
	VSGraph_SearchAStar<VSHeuristic>::VSGraph_SearchAStar(VSGraph * pGraph,VSGraphNode * pSource,VSGraphNode * pTarget)
		:VSGraph_SearchCommon(pGraph,pSource,pTarget)
	{
		VSList<VSGraphNode*>& List = pGraph->GetAllNode();
		VSList<VSGraphNode*>::VSListIterator k = List.Begin();
		for(;k.IsValid() ; ++k)
		{
			m_CostFToThisNode.AddElement((*k),0.0f);
			m_CostGToThisNode.AddElement((*k),0.0f);
		}
		Search();
	}
	template<class VSHeuristic>
	VSGraph_SearchAStar<VSHeuristic>::~VSGraph_SearchAStar()
	{

	}
	template<class VSHeuristic>
	void VSGraph_SearchAStar<VSHeuristic>::Search()
	{
		SortElement SE;
		SE.m_fCost = 0.0f;
		SE.m_pGraphNode = m_pSource;
		m_Priority.AddElement(SE);
		while(m_Priority.GetNum())
		{
			VSGraphNode * pNextNode = m_Priority[m_Priority.GetNum() - 1].m_pGraphNode;
			m_Priority.Erase(m_Priority.GetNum() - 1);
			unsigned int uiIndex = RemoveEdge(pNextNode);
			if (pNextNode == m_pTarget)
			{
				return ;
			}
			VSList<VSGraphEdge*>& List  = pNextNode->GetEdge();
			VSList<VSGraphEdge*>::VSListIterator k = List.Begin();
			//unsigned int uiIndex = m_CostGToThisNode.Find(pNextNode);
			float fGCost = m_CostGToThisNode[uiIndex].Value; 
			for(;k.IsValid() ; ++k)
			{
				float fNewCost = fGCost + (*k)->GetCost();
				float fHCost = VSHeuristic::Calculate(m_pTarget,(*k)->GetTo());
				unsigned int uiIndex1 = m_NodeCurShortEdge.Find((*k)->GetTo());
				if (m_NodeCurShortEdge[uiIndex1].Value == NULL)
				{
					//unsigned int uiIndex2 = m_CostFToThisNode.Find((*k)->GetTo());
					m_CostFToThisNode[uiIndex1].Value = fNewCost + fHCost;

					//unsigned int uiIndex3 = m_CostGToThisNode.Find((*k)->GetTo());
					m_CostGToThisNode[uiIndex1].Value = fNewCost;

					SortElement SE;
					SE.m_fCost = fNewCost + fHCost;
					SE.m_pGraphNode = (*k)->GetTo();
					m_Priority.AddElement(SE);
					m_NodeCurShortEdge[uiIndex1].Value = (*k);
				}
				else 
				{
					//unsigned int uiIndex2 = m_NodePathShortestEdge.Find((*k)->GetTo());
					if (m_NodePathShortestEdge[uiIndex1].Value == NULL)
					{
						//unsigned int uiIndex3 = m_CostGToThisNode.Find((*k)->GetTo());
						if(m_CostGToThisNode[uiIndex1].Value > fNewCost)
						{
							m_CostGToThisNode[uiIndex1].Value  = fNewCost;
							m_CostGToThisNode[uiIndex1].Value  = fNewCost + fHCost;
							ReSort((*k)->GetTo(),fNewCost + fHCost);
							m_NodeCurShortEdge[uiIndex1].Value = (*k);
						}
					}
				}
			}

		}
	}
	template<class VSHeuristic>
	float VSGraph_SearchAStar<VSHeuristic>::GetCostToTarget()const
	{
		if (!m_pTarget)
		{
			return 0.0f;
		}
		unsigned int uiIndex = m_CostGToThisNode.Find(m_pTarget);
		return m_CostGToThisNode[uiIndex].Value;
	}
}
