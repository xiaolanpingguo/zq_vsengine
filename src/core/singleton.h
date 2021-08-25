#pragma once


namespace zq
{

template <typename T> 
class Singleton
{
	static T *m_pSingleton;

public:
	Singleton()
	{
		VSMAC_ASSERT( !m_pSingleton );
		m_pSingleton = static_cast<T*>(this);
	}

	~Singleton()
	{
		VSMAC_ASSERT( m_pSingleton );
		m_pSingleton = NULL;
	}

	static T &GetSingleton()
	{ 
		VSMAC_ASSERT( m_pSingleton );
		return (*m_pSingleton); 
	}

	static T *GetSingletonPtr()
	{ 
		return (m_pSingleton);
	}
};

template <typename T> T *Singleton<T>::m_pSingleton = NULL;

}

#endif //__C_SINGLETON_H__
