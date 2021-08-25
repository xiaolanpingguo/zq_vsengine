#pragma once


#include "core/synchronize.h"


namespace zq{

	

    template <class T>
    class VSPointer
    {
    public:

        VSPointer(T* pObject = 0);
        VSPointer(const VSPointer& rPointer);
        ~VSPointer();


        T& operator* () const;
        T* operator-> () const;


        VSPointer& operator= (T* pObject);
        VSPointer& operator= (const VSPointer& rReference);


        bool operator== (T* pObject) const;
        bool operator!= (T* pObject) const;
        bool operator== (const VSPointer& rReference) const;
        bool operator!= (const VSPointer& rReference) const;

        bool operator > (T* pObject) const;
        bool operator < (T* pObject) const;
        bool operator > (const VSPointer& rReference) const;
        bool operator < (const VSPointer& rReference) const;

        operator T*()const;

        inline T * GetObject()const
        {
            return m_pObject;
        }
        inline void * GetObjectAddress()const
        {
            return (void *)&m_pObject;
        }
        friend class VSStream;
    private:
        T* m_pObject;
        inline void SetObject(T *  pObject)
        {
            m_pObject = pObject;
        }

    };



    template <class T>
    VSPointer<T>::VSPointer(T* pObject)
    {
        m_pObject = pObject;
        if (m_pObject)
        {
            m_pObject->IncreRef();
        }
    }
    //----------------------------------------------------------------------------
    template <class T>
    VSPointer<T>::VSPointer(const VSPointer& rPointer)
    {
        m_pObject = rPointer.m_pObject;
        if (m_pObject)
        {
            m_pObject->IncreRef();

        }
    }
    //----------------------------------------------------------------------------
    template <class T>
    VSPointer<T>::~VSPointer()
    {
        if (m_pObject)
        {
            m_pObject->DecreRef();
            m_pObject = NULL;
        }
    }
    //----------------------------------------------------------------------------

    template <class T>
    T& VSPointer<T>::operator* () const
    {
        return *m_pObject;
    }
    //----------------------------------------------------------------------------
    template <class T>
    T* VSPointer<T>::operator-> () const
    {
        return m_pObject;
    }
    //----------------------------------------------------------------------------
    template <class T>
    VSPointer<T>& VSPointer<T>::operator= (T* pObject)
    {
        if (m_pObject != pObject)
        {
            if (pObject)
            {
                pObject->IncreRef();
            }

            if (m_pObject)
            {
                m_pObject->DecreRef();
            }

            m_pObject = pObject;
        }
        return *this;
    }
    //----------------------------------------------------------------------------
    template <class T>
    VSPointer<T>& VSPointer<T>::operator= (const VSPointer& rPointer)
    {
        if (m_pObject != rPointer.m_pObject)
        {
            if (rPointer.m_pObject)
            {
                rPointer.m_pObject->IncreRef();
            }

            if (m_pObject)
            {
                m_pObject->DecreRef();
            }

            m_pObject = rPointer.m_pObject;
        }
        return *this;
    }
    //----------------------------------------------------------------------------
    template <class T>
    bool VSPointer<T>::operator== (T* pObject) const
    {
        return (m_pObject == pObject);
    }
    //----------------------------------------------------------------------------
    template <class T>
    bool VSPointer<T>::operator!= (T* pObject) const
    {
        return (m_pObject != pObject);
    }
    //----------------------------------------------------------------------------
    template <class T>
    bool VSPointer<T>::operator== (const VSPointer& rPointer) const
    {
        return (m_pObject == rPointer.m_pObject);
    }
    //----------------------------------------------------------------------------
    template <class T>
    bool VSPointer<T>::operator!= (const VSPointer& rPointer) const
    {
        return (m_pObject != rPointer.m_pObject);
    }
    //----------------------------------------------------------------------------
    template <class T>
    VSPointer<T>::operator T *()const
    {
        return m_pObject;
    }
    template <class T>
    bool VSPointer<T>::operator > (T* pObject) const
    {
        return (ComparePointer(m_pObject, pObject) > 0);
    }
    //----------------------------------------------------------------------------
    template <class T>
    bool VSPointer<T>::operator < (T* pObject) const
    {
        return (ComparePointer(m_pObject, pObject) < 0);
    }
    //----------------------------------------------------------------------------
    template <class T>
    bool VSPointer<T>::operator > (const VSPointer& rPointer) const
    {
        return (ComparePointer(m_pObject, rPointer.m_pObject) > 0);
    }
    //----------------------------------------------------------------------------
    template <class T>
    bool VSPointer<T>::operator < (const VSPointer& rPointer) const
    {
        return (ComparePointer(m_pObject, rPointer.m_pObject) < 0);
    }

#define DECLARE_Ptr(ClassName)\
	class ClassName;\
	typedef VSPointer<ClassName> ##ClassName##Ptr;
}
