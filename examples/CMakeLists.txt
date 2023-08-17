cmake_minimum_required( VERSION 3.14 )

#----------------------------------------------------------------------------------------------------------------------
# general settings and options
#----------------------------------------------------------------------------------------------------------------------

include("cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

add_executable(libmath-las-example)


#----------------------------------------------------------------------------------------------------------------------
# dependencies
#----------------------------------------------------------------------------------------------------------------------

target_link_libraries(libmath-las-example PRIVATE libmath::libmath)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(libmath-las-example libmath::libmath)
endif()

#----------------------------------------------------------------------------------------------------------------------
# sources
#----------------------------------------------------------------------------------------------------------------------

set(LASExampleSources
    solvers/las.cpp
)

#----------------------------------------------------------------------------------------------------------------------
# target
#----------------------------------------------------------------------------------------------------------------------

target_sources ( libmath-las-example PRIVATE ${LASExampleSources} )

target_include_directories( libmath-las-example
  PUBLIC
    "$<INSTALL_INTERFACE:${CMAKE_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
  PRIVATE
    "$<INSTALL_INTERFACE:${CMAKE_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
)
