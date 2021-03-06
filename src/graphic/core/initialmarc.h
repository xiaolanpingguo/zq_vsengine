#define DECLARE_INITIAL \
public: \
		static bool RegisterMainFactory(); \
public: \
		static bool InitialClassFactory(); \
		static VSObject * FactoryFunc(); \
		static bool ms_bRegisterMainFactory; \
		static bool InitialProperty(VSRtti *); \
		static bool TerminalProperty();
/********************************************************************************/		
#define IMPLEMENT_INITIAL_BEGIN(classname) \
	static bool gs_bStreamRegistered_##classname= classname::RegisterMainFactory (); \
	bool classname::ms_bRegisterMainFactory = false; \
	bool classname::InitialClassFactory() \
	{ \
		ms_ClassFactory.AddElement(ms_Type.GetName(),FactoryFunc); \
		return 1; \
	} \
	VSObject * classname::FactoryFunc() \
	{ \
		return MEM_NEW classname;\
	} \
	bool classname::RegisterMainFactory() \
	{ \
		if (!ms_bRegisterMainFactory) \
		{  

/********************************************************************************/		
#define IMPLEMENT_INITIAL_END \
			VSMain::AddInitialFunction(InitialClassFactory); \
			VSMain::AddInitialPropertyFunction(InitialProperty); \
			VSMain::AddTerminalPropertyFunction(TerminalProperty); \
			ms_bRegisterMainFactory = true; \
		} \
		return ms_bRegisterMainFactory; \
	}
/********************************************************************************/	
#define DECLARE_INITIAL_ONLY \
public: \
		static bool RegisterMainFactory(); \
public: \
		static bool ms_bRegisterMainFactory;
/********************************************************************************/			
#define IMPLEMENT_INITIAL_ONLY_BEGIN(classname) \
	static bool gs_bStreamRegistered_##classname= classname::RegisterMainFactory (); \
	bool classname::ms_bRegisterMainFactory = false; \
	bool classname::RegisterMainFactory() \
	{ \
		if (!ms_bRegisterMainFactory) \
		{  

/********************************************************************************/		
#define IMPLEMENT_INITIAL_ONLY_END \
			ms_bRegisterMainFactory = true; \
		} \
		return ms_bRegisterMainFactory; \
	}

/********************************************************************************/
#define DECLARE_INITIAL_NO_CLASS_FACTORY \
public: \
		static bool RegisterMainFactory(); \
public: \
		static bool ms_bRegisterMainFactory; \
		static bool InitialProperty(VSRtti *); \
		static bool TerminalProperty();
/********************************************************************************/		
#define IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(classname) \
	static bool gs_bStreamRegistered_##classname= classname::RegisterMainFactory (); \
	bool classname::ms_bRegisterMainFactory = false; \
	bool classname::RegisterMainFactory() \
	{ \
		if (!ms_bRegisterMainFactory) \
		{  

/********************************************************************************/		
#define IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END \
			VSMain::AddInitialPropertyFunction(InitialProperty); \
			VSMain::AddTerminalPropertyFunction(TerminalProperty); \
			ms_bRegisterMainFactory = true; \
		} \
		return ms_bRegisterMainFactory; \
	}
/********************************************************************************/
#define	ADD_INITIAL_FUNCTION(function_name)\
	VSMain::AddInitialFunction(function_name); 
/********************************************************************************/
#define	ADD_INITIAL_FUNCTION_WITH_PRIORITY(function_name)\
	VSMain::AddInitialFunction(function_name,&ms_Priority); 
/********************************************************************************/
#define	ADD_TERMINAL_FUNCTION(function_name)\
	VSMain::AddTerminalFunction(function_name);
/********************************************************************************/
#define	ADD_TERMINAL_FUNCTION_WITH_PRIORITY(function_name)\
	VSMain::AddTerminalFunction(function_name,&ms_Priority);	
