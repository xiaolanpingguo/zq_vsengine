message(STATUS "CMake math")



CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)

list(APPEND PRIVATE_SOURCES)
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})


add_library(math ${LIB_TYPE} ${PRIVATE_SOURCES})


CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)

target_include_directories(math
  PUBLIC
    ${CMAKE_BINARY_DIR}
    ${PUBLIC_INCLUDES}
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(math PROPERTIES FOLDER "src")
set(LIBRARY_OUTPUT_PATH ${LibOutPutDir})


