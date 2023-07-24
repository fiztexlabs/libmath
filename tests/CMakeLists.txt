cmake_minimum_required( VERSION 3.14 )

#----------------------------------------------------------------------------------------------------------------------
# general settings and options
#----------------------------------------------------------------------------------------------------------------------

include("cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
endif()

#----------------------------------------------------------------------------------------------------------------------
# testing framework
#----------------------------------------------------------------------------------------------------------------------

#include(FetchContent)
#FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz)

#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # do not override parent project's runtime settings on Windows
#set(INSTALL_GTEST OFF)

## For simplicity, always build googletest as static library. This prevents mylib-tests executable from
## complaining about missing googletest DLLs on Windows.
# set(BUILD_SHARED_LIBS OFF)

#FetchContent_MakeAvailable(googletest)

# build gtest from source tree
# add_subdirectory(./googletest)

add_executable ( libmath-basic-test )
add_executable ( libmath-debug-test )

#----------------------------------------------------------------------------------------------------------------------
# tests dependencies
#----------------------------------------------------------------------------------------------------------------------

if(is_top_level)
    # set(CMAKE_PREFIX_PATH "d:/Users/BARINOV/UsrCppLibs")
    # set(CMAKE_INSTALL_PREFIX "d:/Users/Konovalov/MyCppLibs")
    # find_package(math REQUIRED)
endif()

if(GTest_FOUND)
   target_link_libraries( libmath-basic-test
       PRIVATE
           libmath::libmath
           GTest::GTest
           GTest::Main
   )
   target_link_libraries( libmath-debug-test
       PRIVATE
           libmath::libmath
           GTest::GTest
           GTest::Main
   )
else()
   target_link_libraries( libmath-basic-test
       PRIVATE
           libmath::libmath
           gtest
           gtest_main
    )
   target_link_libraries( libmath-debug-test
       PRIVATE
           libmath::libmath
           gtest
           gtest_main
    )
endif()

target_link_libraries(libmath-debug-test PRIVATE libmath::libmath)


if(NOT is_top_level)
    win_copy_deps_to_target_dir(libmath-basic-test libmath::libmath)
    win_copy_deps_to_target_dir(libmath-debug-test libmath::libmath)
endif()

#----------------------------------------------------------------------------------------------------------------------
# tests sources
#----------------------------------------------------------------------------------------------------------------------

set(BasicTestSources
    #tests_main.cc 
    basics/test_basics.cpp
)

set(DebugTestSources
    debug/test_debug.cpp
)

#----------------------------------------------------------------------------------------------------------------------
# tests target
#----------------------------------------------------------------------------------------------------------------------

target_sources ( libmath-basic-test PRIVATE ${BasicTestSources} )

target_include_directories( libmath-basic-test
  PUBLIC
    "$<INSTALL_INTERFACE:${CMAKE_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
  PRIVATE
    "$<INSTALL_INTERFACE:${CMAKE_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
)

target_sources ( libmath-debug-test PRIVATE ${DebugTestSources} )

target_include_directories( libmath-debug-test
  PUBLIC
    "$<INSTALL_INTERFACE:${CMAKE_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
  PRIVATE
    "$<INSTALL_INTERFACE:${CMAKE_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
)
