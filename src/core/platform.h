#pragma once


#include <stdint.h>


#ifdef _MSC_VER
#  define COMPILER_MSC
#elif defined( __INTEL_COMPILER )
#  define COMPILER_INTEL
#elif defined( __BORLANDC__ )
#  define COMPILER_BORLAND
#elif defined( __GNUC__ )
#  define COMPILER_GNU
#elif defined( __clang__ )
#  define COMPILER_CLANG
#else
#  pragma error "FATAL ERROR: Unknown compiler."
#endif


#ifndef X64
#if defined( _WIN64 ) || defined( __x86_64__ ) || defined( __amd64 ) || defined( __LP64__ )
#define X64
#endif
#endif


#ifdef COMPILER_MSC
	#define DLL_EXPORT __declspec(dllexport)
	#define DLL_IMPORT __declspec(dllimport)
#elif COMPILER_GNU
	#define DLL_EXPORT __attribute__((visibility("default")))
	#define DLL_IMPORT
#else
	#error compiler not supported!
#endif



namespace zq{


#ifdef PLATFORM_WIN
    typedef signed __int64											int64;
    typedef signed __int32											int32;
    typedef signed __int16											int16;
    typedef signed __int8											int8;
    typedef unsigned __int64										uint64;
    typedef unsigned __int32										uint32;
    typedef unsigned __int16										uint16;
    typedef unsigned __int8											uint8;
#else
    typedef ::int64_t												int64;
    typedef ::int32_t												int32;
    typedef ::int16_t												int16;
    typedef ::int8_t												int8;
    typedef ::uint64_t												uint64;
    typedef ::uint32_t												uint32;
    typedef ::uint16_t												uint16;
    typedef ::uint8_t												uint8;
#endif



}

