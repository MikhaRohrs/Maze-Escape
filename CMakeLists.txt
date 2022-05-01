cmake_minimum_required(VERSION 3.11)
#Require modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Maze_Escape)

#### Setup Directories ####

## Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})


#### Add External Dependencies ####

## SFML lib
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} 
    -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res)
	
## Physics lib
add_subdirectory("lib/b2d/Box2D" EXCLUDE_FROM_ALL)
#include_directories("lib/b2d/Box2D/")
set(B2D_INCS "lib/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

## Tile loader lib
file(GLOB_RECURSE SOURCE_FILES lib/tile_level_loader/*.cpp tile_level_loader/*.h)
add_library(tile_level_loader STATIC ${SOURCE_FILES})
target_include_directories(tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib/tile_level_loader/" )
target_link_libraries(tile_level_loader maths sfml-graphics)

## Maths lib
add_library(maths INTERFACE)
target_sources(maths INTERFACE "${CMAKE_SOURCE_DIR}/lib/maths/maths.h")
target_include_directories(maths INTERFACE "${CMAKE_SOURCE_DIR}/lib/maths" SYSTEM INTERFACE ${SFML_INCS})

## ECM lib
file(GLOB_RECURSE SOURCE_FILES lib/ecm/*.cpp ecm/*.h)
add_library(ecm STATIC ${SOURCE_FILES})
target_include_directories(ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib/ecm" )
target_link_libraries(ecm PRIVATE maths)

## Engine lib
file(GLOB_RECURSE RESOURCE_FILES "res/*.*")
file(GLOB_RECURSE SOURCE_FILES lib/engine/*.cpp lib/engine/*.h)
add_library(engine STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(engine INTERFACE "${CMAKE_SOURCE_DIR}/lib/engine/" PUBLIC SYSTEM ${B2D_INCS} )
target_link_libraries(engine tile_level_loader maths ecm Box2D sfml-graphics sfml-audio)


#### Maze Escape ####
file(GLOB_RECURSE SOURCES maze_escape/*.cpp maze_escape/*.h)
file(GLOB_RECURSE CMPNTS maze_escape/components/*.cpp maze_escape/components/*.h)
file(GLOB_RECURSE SCENES maze_escape/scenes/*.cpp maze_escape/scenes/*.h)
add_executable(MAZE_ESCAPE ${SOURCES} ${RESOURCE_FILES})
source_group("components" FILES ${CMPNTS})
source_group("resources" FILES ${RESOURCE_FILES})
target_include_directories(MAZE_ESCAPE SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS})
target_link_libraries(MAZE_ESCAPE engine)
set(EXECUTABLES ${EXECUTABLES} MAZE_ESCAPE)

add_dependencies(MAZE_ESCAPE copy_resources)

set_target_properties(MAZE_ESCAPE 
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)