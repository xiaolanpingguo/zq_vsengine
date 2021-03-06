#pragma once


#include "graphic/core/object.h"


namespace zq{


class VSThreadCommand
{
public:
	VSThreadCommand(bool bMustFlush = false):m_bMustFlush(bMustFlush){}
	virtual ~VSThreadCommand(){}
	virtual uint32 Execute() = 0;
	virtual const char * DescribeCommand() = 0;
	inline bool MustFlush() const
	{
		return m_bMustFlush;
	}
protected:
	bool m_bMustFlush;
};


class GRAPHIC_API VSConstantBuffer : public MemObject
{
public:
	enum
	{
		Constant_BUFFER_SIZE = 6 * 1024 * 1024
	};
	//VSUserConstant Type
	VSConstantBuffer();
	~VSConstantBuffer();
	unsigned char * Assign(uint32 uiSize);
	unsigned char * Assign(uint32 VTType,uint32 uiRegisterNum);
	void Clear();
	friend class VSRenderThreadBuffer;
protected:
	VSCriticalSection m_Lock;
	VSArray<unsigned char> m_Buffer;
	uint32 m_uiCurBufferP;
};
#define DECLARE_UNIQUE_THREAD_COMMAND(TypeName,Code) \
class TypeName : public VSThreadCommand \
{ \
public: \
	TypeName() \
	{}\
	~TypeName()\
	{ \
	} \
	virtual uint32 Execute() \
	{ \
		Code; \
		return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
		return _T( #TypeName ); \
	} \
};

#define DECLARE_UNIQUE_THREAD_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,Code) \
class TypeName : public VSThreadCommand \
{ \
public: \
	typedef ParamType1 _ParamType1; \
	TypeName() \
	{}\
	TypeName(const _ParamType1& In##ParamName1): \
		ParamName1(In##ParamName1) \
	{} \
	~TypeName()\
	{ \
	} \
	virtual uint32 Execute() \
	{ \
		Code; \
		return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
		return _T( #TypeName ); \
	} \
private: \
	ParamType1 ParamName1; \
}; 
#define DECLARE_UNIQUE_THREAD_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,Code) \
class TypeName : public VSThreadCommand \
{ \
public: \
	typedef ParamType1 _ParamType1; \
	typedef ParamType2 _ParamType2; \
	TypeName() \
	{}\
	~TypeName()\
	{ \
	} \
	TypeName(const _ParamType1& In##ParamName1,const _ParamType2& In##ParamName2): \
		ParamName1(In##ParamName1), \
		ParamName2(In##ParamName2) \
	{} \
	virtual uint32 Execute() \
	{ \
		Code; \
		return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
		return _T( #TypeName ); \
	} \
private: \
	ParamType1 ParamName1; \
	ParamType2 ParamName2; \
};
#define DECLARE_UNIQUE_THREAD_COMMAND_THREEPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,ParamType3,ParamName3,Code) \
class TypeName : public VSThreadCommand \
{ \
public: \
	typedef ParamType1 _ParamType1; \
	typedef ParamType2 _ParamType2; \
	typedef ParamType3 _ParamType3; \
	TypeName() \
	{}\
	TypeName(const _ParamType1& In##ParamName1,const _ParamType2& In##ParamName2,const _ParamType3& In##ParamName3): \
		ParamName1(In##ParamName1), \
		ParamName2(In##ParamName2), \
		ParamName3(In##ParamName3) \
	{} \
	~TypeName()\
	{ \
	} \
	virtual uint32 Execute() \
	{ \
		Code; \
		return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
		return _T( #TypeName ); \
	} \
private: \
	ParamType1 ParamName1; \
	ParamType2 ParamName2; \
	ParamType3 ParamName3; \
};

#define DECLARE_UNIQUE_THREAD_COMMAND_FOURPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,ParamType3,ParamName3,ParamType4,ParamName4,Code) \
class TypeName : public VSThreadCommand \
{ \
public: \
	typedef ParamType1 _ParamType1; \
	typedef ParamType2 _ParamType2; \
	typedef ParamType3 _ParamType3; \
	typedef ParamType4 _ParamType4; \
	TypeName() \
	{}\
	TypeName(const _ParamType1& In##ParamName1, const _ParamType2& In##ParamName2, const _ParamType3& In##ParamName3, const _ParamType4& In##ParamName4) : \
	ParamName1(In##ParamName1), \
	ParamName2(In##ParamName2), \
	ParamName3(In##ParamName3), \
	ParamName4(In##ParamName4) \
	{} \
	~TypeName()\
	{ \
	} \
	virtual uint32 Execute() \
	{ \
	Code; \
	return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
	return _T(#TypeName); \
	} \
private: \
	ParamType1 ParamName1; \
	ParamType2 ParamName2; \
	ParamType3 ParamName3; \
	ParamType4 ParamName4; \
};

template<typename TypeName ,typename LAMBDA>
class VSLambdaCommand : public VSThreadCommand
{
public:
	VSLambdaCommand(LAMBDA && InLambda,bool bMustFlush = false) : VSThreadCommand(bMustFlush), lambda(InLambda)
	{}
 	VSLambdaCommand()
 	{}
	~VSLambdaCommand()
	{
	}
	virtual uint32 Execute()
	{
		lambda();
		return sizeof(*this);
	}
	virtual const char* DescribeCommand()
	{
		return TypeName::GetName();
	}
private:
	LAMBDA lambda;
};
}
