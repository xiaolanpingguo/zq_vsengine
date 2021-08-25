#pragma once
#include "graphic/core/object.h"
namespace zq
{
	struct PropertyReplaceType
	{
		VSArrayOrder<VSUsedName> ReplaceProperty;
	};
	struct ClassReplaceType
	{
		VSArrayOrder<VSUsedName> ReplaceClass;
		VSMap<VSUsedName, PropertyReplaceType> ProperyReplaceMap;
	};
	class GRAPHIC_API VSConfig
	{
		//PRIORITY
		DECLARE_PRIORITY
	public:
		VSConfig();
		~VSConfig();
		DECLARE_INITIAL_ONLY
		typedef VSMap < VSUsedName, ClassReplaceType> CLASS_REPLACE_TYPE;
	private:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		static void InitConfig();
		static void InitEnginePath();
		static void InitParamConfig();
		static void InitClassReplace();
		static void Inner_InitClassReplace();
		static CLASS_REPLACE_TYPE ms_ClassReplaceMap;
		static VSMap<VSUsedName, VSUsedName> ms_ClassReplace;
		static VSMap<VSUsedName, VSMap<unsigned int, unsigned int>> ms_PropertyReplace;
	public:
		
		static bool ms_EnableAsynLoad;
		static bool ms_EnableAdvanceInstance;
		static VSString ms_OutputShaderCodePath;
		static VSString ms_OutputLogPath;
		static VSString ms_TerrainPath;
		static VSString ms_ResourcePath;
		static float ms_LODScreenScale;
		

		static bool GetPropertyReplace(VSString & ClassName, VSMap<unsigned int, unsigned int> * &pPropertyReplace);
		static bool GetClassReplace(VSString & ClassName);
		static bool GetPropertyReplace(VSMap<unsigned int, unsigned int> * pPropertyReplace, uint32 &NameID);
	};
}
