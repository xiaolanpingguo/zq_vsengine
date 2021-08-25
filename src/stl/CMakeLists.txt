message(STATUS "CMake stl")



CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)

list(APPEND PRIVATE_SOURCES)
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})


add_library(stl ${LIB_TYPE} ${PRIVATE_SOURCES})


CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)

target_include_directories(stl
  PUBLIC
    ${CMAKE_BINARY_DIR}
    ${PUBLIC_INCLUDES}
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(stl PROPERTIES FOLDER "src")
set(LIBRARY_OUTPUT_PATH ${LibOutPutDir})


