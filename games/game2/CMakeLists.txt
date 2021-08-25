MESSAGE(STATUS "CMake game2")


CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)

list(APPEND PRIVATE_SOURCES)
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})


add_executable(game2 WIN32 ${PRIVATE_SOURCES})


CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)

target_include_directories(game2
  PUBLIC
    ${CMAKE_BINARY_DIR}
    ${PUBLIC_INCLUDES}
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})


target_link_libraries(game2 core math datastruct stl graphic 
	${SolutionDir}/src/thirdparty/FBX/Lib/vs2017/x64/release/libfbxsdk.lib
	${SolutionDir}/src/thirdparty/NVCompress/Lib/nvtt_64.lib
	${SolutionDir}/src/thirdparty/TBB/Lib/tbb.lib
	${platform_link_lib})

set(EXECUTABLE_OUTPUT_PATH ${BinOutPutDir})
set_target_properties(game2 PROPERTIES FOLDER "games")