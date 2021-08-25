cmake_minimum_required(VERSION 3.1)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(zq_game_engine)

message(STATUS "project source dir = ${PROJECT_SOURCE_DIR}")
message(STATUS "project bin dir = ${PROJECT_BINARY_DIR}")
message(STATUS "CMAKE_SYSTEM_NAME          = ${CMAKE_SYSTEM_NAME}"         )
message(STATUS "CMAKE_CXX_COMPILER_ID      = ${CMAKE_CXX_COMPILER_ID}"     )
message(STATUS "CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_COMPILER_IS_GNUCXX   = ${CMAKE_COMPILER_IS_GNUCXX}"  )

set(SolutionDir ${PROJECT_SOURCE_DIR})

# vs的工程筛选，可以将多个工程筛选到一个属性内
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

# set(INSTALL_INCLUDE_DIR ${SolutionDir}/include/zq_game_engine)
# set(INSTALL_LIB_DIR ${SolutionDir}/lib)
set(INSTALL_INCLUDE_DIR ${SolutionDir}/install/zq_game_engine)
set(INSTALL_LIB_DIR ${SolutionDir}/install/lib)
set(INSTALL_DLL_DIR ${SolutionDir}/install/dll)

option(DEBUG "ON for Debug or OFF for release" ON)
set(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug")
message(STATUS "debug or release:" ${CMAKE_BUILD_TYPE})


list(APPEND CMAKE_MODULE_PATH 
	"${CMAKE_SOURCE_DIR}/cmake"
	"${CMAKE_SOURCE_DIR}/cmake/macros")
include(CheckCXXSourceRuns)
include(CheckIncludeFiles)
include(ConfigureScripts)
include(cmake/options.cmake) # showoptions.cmake影响vs的筛选器
include(ConfigureBaseTargets)
include(CheckPlatform)
include(GroupSources)
include(AutoCollect)
include(cmake/showoptions.cmake) 

# static or dynamic
set(LIB_TYPE STATIC)
set(USE_STATIC 1)
if (USE_STATIC)
	message(STATUS "build static lib")
	set(LIB_TYPE STATIC) 
else()
	message(STATUS "build dynamic lib")
	set(LIB_TYPE SHARED) 
endif()


if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()
	

if (WIN32)
	
	add_definitions(
		-DPLATFORM_WIN
		)
		
	if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	
		set(LibOutPutDir ${SolutionDir}/_libs)		
		set(BinOutPutDir ${SolutionDir}/bin)
		
	else()
	
		set(LibOutPutDir ${SolutionDir}/_libs)	
		set(BinOutPutDir ${SolutionDir}/bin)
	
	endif()
	
	list(APPEND platform_link_lib d3dcompiler.lib dxguid.lib dxgi.lib D3D11.lib)
	

else(UNIX)

	add_definitions(-D PLATFORM_UNIX)
	
	if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	
		set(LibOutPutDir ${SolutionDir}/_libs/Debug)		
		set(BinOutPutDir ${SolutionDir}/bin/Debug)
		
	else()
	
		set(LibOutPutDir ${SolutionDir}/_libs/Release)	
		set(BinOutPutDir ${SolutionDir}/bin/Release)
	
	endif()
endif()


include_directories(
	${SolutionDir}/src
	#${SolutionDir}/src/thirdparty/DX11/Include
	#${SolutionDir}/src/thirdparty/vulkan
	#${SolutionDir}/src/thirdparty/vulkan/include
	#${SolutionDir}/src/thirdparty/vulkan/loader
)


link_directories(
	#${SolutionDir}/src/thirdparty/DX11/libs
	${LibOutPutDir}
	)

	

add_subdirectory(src)
add_subdirectory(games)


#file(GLOB base_code ${SolutionDir}/zq_engine/baselib/base_code/*.h*)
#install(FILES ${base_code} DESTINATION ${CMAKE_INSTALL_PREFIX}/base_code/)FILES
install(DIRECTORY ${SolutionDir}/zq_game_engine/include/zq_game_engine DESTINATION ${INSTALL_INCLUDE_DIR}/)
install(DIRECTORY  ${SolutionDir}/_libs/ DESTINATION ${INSTALL_LIB_DIR}/)




