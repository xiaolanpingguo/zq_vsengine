#pragma once


#include "graphic/render/scenemanager/scenerender.h"
#include "graphic/render/scenemanager/viewfamily.h"


namespace zq{


class GRAPHIC_API VSSceneManager : public MemObject
{
public:
	VSSceneManager();
	~VSSceneManager();

    inline void AddScene(VSScene * pSecen)
    {
        if (pSecen)
        {
            for (uint32 i = 0; i < m_pScene.GetNum(); i++)
            {
                if (m_pScene[i] == pSecen)
                {
                    return;
                }
            }

            m_pScene.AddElement(pSecen);
        }
    }

    inline VSScene* GetScene(uint32 i)const
    {
        if (i >= m_pScene.GetNum())
            return NULL;
        return m_pScene[i];
    }

    inline void ClearAllScene()
    {
        m_pScene.Clear();
    }

    inline void DeleteScene(uint32 i)
    {
        if (i >= m_pScene.GetNum())
            return;
        m_pScene.Erase(i);

    }

    inline void DeleteScene(VSScene * pScene)
    {
        if (!pScene)
        {
            return;
        }
        for (uint32 i = 0; i < m_pScene.GetNum(); i++)
        {
            if (pScene == m_pScene[i])
            {
                m_pScene.Erase(i);
            }
        }
    }

    inline uint32 GetSceneNum()const
    {
        return m_pScene.GetNum();
    }

    inline void AddViewFamily(VSViewFamily * pViewFamily)
    {
        if (pViewFamily)
        {
            for (uint32 i = 0; i < m_pViewFamily.GetNum(); i++)
            {
                if (m_pViewFamily[i] == pViewFamily)
                {
                    return;
                }
            }

            m_pViewFamily.AddElement(pViewFamily);
        }
    }

    inline VSViewFamily* GetViewFamily(uint32 i)const
    {
        if (i >= m_pViewFamily.GetNum())
            return NULL;
        return m_pViewFamily[i];
    }

    VSViewFamily* GetViewFamily(const VSString & ViewFamilyName) const
    {
        for (uint32 i = 0; i < m_pViewFamily.GetNum(); i++)
        {
            if (m_pViewFamily[i]->m_ViewFamilyName == ViewFamilyName)
            {
                return m_pViewFamily[i];
            }
        }

        return NULL;
    }

    inline void ClearAllViewFamily()
    {
        m_pViewFamily.Clear();
    }

    inline void DeleteViewFamily(uint32 i)
    {
        if (i >= m_pViewFamily.GetNum())
            return;
        m_pViewFamily.Erase(i);
    }

    inline void DeleteViewFamily(VSViewFamily* pViewFamily)
    {
        if (!pViewFamily)
        {
            return;
        }
        for (uint32 i = 0; i < m_pViewFamily.GetNum(); i++)
        {
            if (pViewFamily == m_pViewFamily[i])
            {
                m_pViewFamily.Erase(i);
            }
        }
    }

    inline uint32 GetViewFamilyNum()const
    {
        return m_pViewFamily.GetNum();
    }
		
	void Update(double dAppTime);
	void Draw(double dAppTime);
	static VSSceneManager * ms_pSceneManager;

protected:

	VSArray<VSViewFamily*> m_pViewFamily;
	VSArray<VSScene *> m_pScene;
};
	

}