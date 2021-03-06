
#define DECLARE_RTTI \
public:\
	virtual VSRtti & GetType()const{return ms_Type;}\
	static VSRtti ms_Type;\
public:\
	static  VSPriority ms_Priority;	

#define IMPLEMENT_RTTI(classname,baseclassname)\
	VSRtti classname::ms_Type(_T(#classname),&baseclassname::ms_Type,classname::FactoryFunc); \
	VSPriority classname::ms_Priority;


#define IMPLEMENT_RTTI_NoCreateFun(classname,baseclassname)\
	VSRtti classname::ms_Type(_T(#classname),&baseclassname::ms_Type,NULL); \
	VSPriority classname::ms_Priority;

#define IMPLEMENT_RTTI_NoParent(classname)\
	VSRtti classname::ms_Type(_T(#classname),NULL,classname::FactoryFunc); \
	VSPriority classname::ms_Priority;


#define IMPLEMENT_RTTI_NoParent_NoCreateFun(classname)\
	VSRtti classname::ms_Type(_T(#classname),NULL,NULL); \
	VSPriority classname::ms_Priority;



	