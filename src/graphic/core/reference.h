#pragma once


#include "graphic/core/graphic.h"
#include "core/memorymanager.h"
#include "graphic/core/pointer.h"


namespace zq{


	class GRAPHIC_API VSReference
	{
	public:
		template <class T>
		friend class VSPointer;
		friend class VSVariant;
		VSReference();
		virtual ~VSReference() = 0;
		//SmartPointer
	public:
		int32 GetRef()const { return m_iReference;}
	protected:
		void IncreRef()
		{
			VSLockedIncrement((long *)&m_iReference);
			//m_iReference++;
		}
		void DecreRef()
		{
			//m_iReference--; 
			VSLockedDecrement((long *)&m_iReference);
			if(!m_iReference) 
				MEM_DELETE this;
		}
		int32 m_iReference;
	
	
	};

}
