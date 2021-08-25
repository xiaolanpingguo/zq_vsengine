#pragma once


#include "core/system.h"
#include "graphic/core/graphic.h"


namespace zq
{
	class GRAPHIC_API VSTextureOutputInfo
	{
	public:
		~VSTextureOutputInfo() {};
		inline uint32 GetWidth()const
		{
			return m_uiWidth;
		}
		inline uint32 GetHeight()const
		{

			return m_uiHeight;
		}
		inline uint32 GetFormatType()const
		{
			return m_uiFormatType;
		}
		inline uint32 GetMulSample()const
		{
			return m_uiMulSample;
		}
		inline uint32 GetLevel()const
		{
			return m_uiLevel;
		}
		inline uint32 GetFirst()const
		{
			return m_uiFirst;
		}
		inline uint32 GetArraySize()const
		{
			return m_uiArraySize;
		}
	protected:
		VSTextureOutputInfo() {};
		uint32 m_uiWidth;
		uint32 m_uiHeight;
		uint32 m_uiFormatType;
		uint32 m_uiMulSample;
		uint32 m_uiLevel;
		uint32 m_uiFirst;
		uint32 m_uiArraySize;
	};
}

