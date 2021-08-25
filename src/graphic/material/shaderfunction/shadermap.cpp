#include "graphic/material/shaderfunction/shadermap.h"
#include "graphic/node/geometry.h"
#include "graphic/node/model/bonenode.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI(VSShaderMapCache, VSCacheResource)
BEGIN_ADD_PROPERTY(VSShaderMapCache, VSCacheResource)
REGISTER_PROPERTY(m_ShaderMap,ShaderMap,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSShaderMapCache)
IMPLEMENT_INITIAL_END
VSShaderMap::VSShaderMap(VSString ShaderMapName)
{
	m_ShaderMapName = ShaderMapName;
}
VSShaderMap::~VSShaderMap()
{
	Clear();
}
void VSShaderMap::SetShader(const VSUsedName & Name,const VSShaderKey & Key,VSShader * pShader)
{
	uint32 index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		m_ShaderMap[index].Value.AddElement(Key,pShader);
	}
	else
	{
		VSShaderSet Temp;
		Temp.AddElement(Key,pShader);
		m_ShaderMap.AddElement(Name,Temp);
	}
}
VSShaderSet * VSShaderMap::GetShaderSet(const VSUsedName &Name)
{
	uint32 index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		return &m_ShaderMap[index].Value;
	}
	else
	{
		return NULL;
	}
}
void VSShaderMap::DeleteShaderSet(const VSUsedName & Name)
{
	uint32 index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		m_ShaderMap.Erase(index);
	}
}
VSShader * VSShaderMap::GetShader(const VSUsedName & Name,const VSShaderKey &Key)
{
	uint32 index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		VSShaderSet * pShaderSet = &m_ShaderMap[index].Value;
		uint32 index2 =(*pShaderSet).Find(Key);
		if (index2 != (*pShaderSet).GetNum())
		{
			return (*pShaderSet)[index2].Value;
		}
	}
	return NULL;
}
void VSShaderMap::DeleteShader(const VSUsedName & Name,const VSShaderKey &Key)
{
	uint32 index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		VSShaderSet * pShaderSet = &m_ShaderMap[index].Value;
		uint32 index2 = (*pShaderSet).Find(Key);
		if (index2 != (*pShaderSet).GetNum())
		{
			return (*pShaderSet).Erase(index2);
		}
	}
}