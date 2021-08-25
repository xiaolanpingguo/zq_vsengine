#pragma once
#include "graphic/core/resource.h"
#include "graphic/render/renderer.h"
namespace zq
{
	DECLARE_Ptr(VSShader);
	DECLARE_Ptr(VSShaderKey);
	typedef VSMap<VSShaderKey,VSShaderPtr> VSShaderSet;
	class GRAPHIC_API VSShaderMapCache : public VSCacheResource
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		DECLARE_CACHE_RESOURCE(CacheShader, Resource/Shader, Cache/Shader, true, false)
	public:
		VSShaderMapCache(){}
		~VSShaderMapCache(){}
		VSMap<VSUsedName,VSShaderSet> m_ShaderMap;
	};
	DECLARE_Ptr(VSShaderMapCache);
	VSTYPE_MARCO(VSShaderMapCache);
	class GRAPHIC_API VSShaderMap
	{
		
	public:
		VSShaderMap(VSString ShaderMapName);
		~VSShaderMap();
	protected:

		
		VSMap<VSUsedName,VSShaderSet> m_ShaderMap;
	public:	
		VSMap<VSUsedName,VSShaderSet>&GetShaderMap()
		{
			return m_ShaderMap;
		}
		void SetShader(const VSUsedName & Name,const VSShaderKey & Key,VSShader * pShader);
		VSShaderSet * GetShaderSet(const VSUsedName &Name);
		void DeleteShaderSet(const VSUsedName & Name);
		VSShader * GetShader(const VSUsedName & Name,const VSShaderKey & Key);
		void DeleteShader(const VSUsedName & Name,const VSShaderKey & Key);
		void Clear()
		{
			m_ShaderMap.Clear();
		}
		void GCResource()
		{

		}
		bool IsReleseAll()
		{
			return (!m_ShaderMap.GetNum());
		}
		VSString m_ShaderMapName;
	};
	
}
