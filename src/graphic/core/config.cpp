#include "graphic/core/config.h"
#include "graphic/core/rtti.h"
#include "datastruct/VSMatch.h"


namespace zq{


VSString VSConfig::ms_OutputShaderCodePath;
VSString VSConfig::ms_OutputLogPath;
VSString VSConfig::ms_TerrainPath;
VSString VSConfig::ms_ResourcePath;
bool VSConfig::ms_EnableAsynLoad = true;
bool VSConfig::ms_EnableAdvanceInstance = false;
float VSConfig::ms_LODScreenScale = 1.0f;
VSMap<VSUsedName, VSUsedName> VSConfig::ms_ClassReplace;
VSMap<VSUsedName, VSMap<unsigned int, unsigned int>> VSConfig::ms_PropertyReplace;
VSConfig::CLASS_REPLACE_TYPE VSConfig::ms_ClassReplaceMap;
IMPLEMENT_PRIORITY(VSConfig)
IMPLEMENT_INITIAL_ONLY_BEGIN(VSConfig);
ADD_PRIORITY(VSRtti);
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_ONLY_END
bool VSConfig::TerminalDefaultState()
{
	return true;
}
bool VSConfig::InitialDefaultState()
{
	InitConfig();
	return true;
}
VSConfig::VSConfig()
{

}
VSConfig::~VSConfig()
{

}
void VSConfig::InitEnginePath()
{
	VSMatch Match;
	if (!Match.Open(_T("Resource\\EnginePath.txt")))
	{
		VSMAC_ASSERT(0);
		return;
	}

	//output
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return;
	} 

	if (Match.PatternMatch("['OutputShaderCodePath'][s>0]"))
	{
		ms_OutputShaderCodePath = Match.pstrings[0];
	} 

	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return;
	}

	if (Match.PatternMatch("['OutputLogPath'][s>0]"))
	{
		ms_OutputLogPath = Match.pstrings[0];
	} 


	//Terrain
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return;
	} 

	if (Match.PatternMatch("['TerrainPath'][s>0]"))
	{
		ms_TerrainPath = Match.pstrings[0];
	} 

	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return;
	} 

	if (Match.PatternMatch("['ResourcePath'][s>0]"))
	{
		ms_ResourcePath = Match.pstrings[0];
	}
}
void VSConfig::InitParamConfig()
{
	VSMatch Match;
	if (!Match.Open(_T("Resource\\Config.txt")))
		return;
	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return;
	}
	if (Match.PatternMatch("['EnableAsynLoad'][s>0]"))
	{
		VSString TrueString = _T("true");
		ms_EnableAsynLoad = (Match.pstrings[0] == TrueString);
	}

	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return;
	}
	if (Match.PatternMatch("['EnableAdvanceInstance'][s>0]"))
	{
		VSString TrueString = _T("true");
		ms_EnableAdvanceInstance = (Match.pstrings[0] == TrueString);
	}

	if (!Match.Getline())
	{
		VSMAC_ASSERT(0);
		return;
	}
	if (Match.PatternMatch("['LODScreenScale'][f]"))
	{
		ms_LODScreenScale = Match.pfloats[0];
	}
}
void VSConfig::InitConfig()
{
	InitEnginePath();
	InitParamConfig();
	InitClassReplace();
}
void VSConfig::InitClassReplace()
{
	VSMatch Match;
	if (!Match.Open(_T("Resource\\ClassReplace.txt")))
		return;
	if (!Match.Getline())
	{
		return;
	} // end if
	while (Match.PatternMatch("[s>1]"))
	{
		uint32 ClassIndex = ms_ClassReplaceMap.AddElement(Match.pstrings[0], ClassReplaceType());
		if (!Match.Getline())
		{
			VSMAC_ASSERT(0);
		} 		
		if (Match.PatternMatch("['{']"))
		{
			if (!Match.Getline())
			{
				VSMAC_ASSERT(0);
			} 
			while (Match.PatternMatch("[s>1]"))
			{
				ms_ClassReplaceMap[ClassIndex].Value.ReplaceClass.AddElement(Match.pstrings[0]);
				if (!Match.Getline())
				{
					VSMAC_ASSERT(0);
				} 
			}
			if (Match.PatternMatch("['[']"))
			{
				if (!Match.Getline())
				{
					VSMAC_ASSERT(0);
				} // end if
				while (Match.PatternMatch("[s>1]"))
				{
					uint32 PropertyIndex = ms_ClassReplaceMap[ClassIndex].Value.ProperyReplaceMap.AddElement(Match.pstrings[0], PropertyReplaceType());
					if (!Match.Getline())
					{
						VSMAC_ASSERT(0);
					}
					if (Match.PatternMatch("['{']"))
					{
						if (!Match.Getline())
						{
							return;
						}
						while (Match.PatternMatch("[s>1]"))
						{
							ms_ClassReplaceMap[ClassIndex].Value.ProperyReplaceMap[PropertyIndex].Value.ReplaceProperty.AddElement(Match.pstrings[0]);
							if (!Match.Getline())
							{
								return;
							}
						}
						if (!Match.PatternMatch("['}']"))
						{
							VSMAC_ASSERT(0);
							return;
						}
						if (!Match.Getline())
						{
							VSMAC_ASSERT(0);
							return;
						}
					}
				}
				if (!Match.PatternMatch("[']']"))
				{
					VSMAC_ASSERT(0);
				}
				if (!Match.Getline())
				{
					VSMAC_ASSERT(0);
				}
			}
			if (!Match.PatternMatch("['}']"))
			{
				VSMAC_ASSERT(0);
			}
			Match.Getline();
		}
	}
	Inner_InitClassReplace();
}
void VSConfig::Inner_InitClassReplace()
{
	for (uint32 i = 0 ; i < ms_ClassReplaceMap.GetNum(); i++)
	{
		VSUsedName ClassName = ms_ClassReplaceMap[i].Key;
		for (uint32 j = 0 ; j < ms_ClassReplaceMap[i].Value.ReplaceClass.GetNum(); j++)
		{
			ms_ClassReplace.AddElement(ms_ClassReplaceMap[i].Value.ReplaceClass[j], ClassName);
		}
		if (ms_ClassReplaceMap[i].Value.ProperyReplaceMap.GetNum() > 0)
		{
			VSMap<unsigned int, unsigned int> Temp;
			uint32 ClassIndex = ms_PropertyReplace.AddElement(ClassName, Temp);
			for (uint32 m = 0 ; m < ms_ClassReplaceMap[i].Value.ProperyReplaceMap.GetNum() ; m ++)
			{
				uint32 PropertyID = ms_ClassReplaceMap[i].Value.ProperyReplaceMap[m].Key.GetNameCode();
				for (uint32 n = 0 ; n < ms_ClassReplaceMap[i].Value.ProperyReplaceMap[m].Value.ReplaceProperty.GetNum() ; n++)
				{
					ms_PropertyReplace[ClassIndex].Value.AddElement(ms_ClassReplaceMap[i].Value.ProperyReplaceMap[m].Value.ReplaceProperty[n].GetNameCode(), PropertyID);
				}
			}
		}
	}
	for (uint32 i = 0 ; i < ms_PropertyReplace.GetNum() ; i++)
	{
		const VSRtti * pRtti = VSRtti::GetRttiByName(ms_PropertyReplace[i].Key);
		const VSRtti * pBaseRtti = pRtti->GetBase();
		while (pBaseRtti)
		{
			uint32 j = ms_PropertyReplace.Find(pBaseRtti->GetName());
			if (j != ms_PropertyReplace.GetNum() && ms_PropertyReplace[j].Value.GetNum())
			{
				ms_PropertyReplace[i].Value.AddElement(ms_PropertyReplace[j].Value,0 , ms_PropertyReplace[j].Value.GetNum() - 1);
			}
			pBaseRtti = pBaseRtti->GetBase();
		}
	}
}
bool VSConfig::GetPropertyReplace(VSString & ClassName, VSMap<unsigned int, unsigned int> * &pPropertyReplace)
{
	VSUsedName Temp = ClassName;
	uint32 i = ms_PropertyReplace.Find(Temp);
	if ( i == ms_PropertyReplace.GetNum())
	{
		return false;
	}
	else
	{
		pPropertyReplace = &ms_PropertyReplace[i].Value;
		return true;
	}
}
bool VSConfig::GetClassReplace(VSString & ClassName)
{
	VSUsedName Temp = ClassName;
	uint32 i = ms_ClassReplace.Find(Temp);
	if(i != ms_ClassReplace.GetNum())
	{
		ClassName = ms_ClassReplace[i].Value.GetString();
		return true;
	}
	return false;

}
bool VSConfig::GetPropertyReplace(VSMap<unsigned int, unsigned int> * pPropertyReplace, uint32 &NameID)
{
	uint32 i = pPropertyReplace->Find(NameID);
	if (i != pPropertyReplace->GetNum())
	{
		NameID = (*pPropertyReplace)[i].Value;
		return true;
	}
	else
	{
		return false;
	}
}

}
