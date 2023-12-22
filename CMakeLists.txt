cmake_minimum_required(VERSION 3.14)
project(libmath
    VERSION 2.0.0
    DESCRIPTION "Math library"
    LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#----------------------------------------------------------------------------------------------------------------------
# general settings and options
#----------------------------------------------------------------------------------------------------------------------

include(cmake/utils.cmake)
include(GNUInstallDirs)
# set(CMAKE_INSTALL_PREFIX "d:/Users/BARINOV/UsrCppLibs")
# set(CMAKE_INSTALL_PREFIX "c:/Users/abarinov/Documents/Work/MyCppLibs")
# set(CMAKE_INSTALL_PREFIX "d:/Users/Konovalov/MyCppLibs")
# set(CMAKE_INSTALL_PREFIX "c:/Users/jo/MyCppLibs")

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

# uncomment for shared build
#set(MATH_SHARED_LIBS " ")

set(CMAKE_BUILD_TYPE Debug)
option(MATH_USE_DOUBLE_PRECISION "Use double precision for calculations" ON)
option(MATH_BUILD_TESTS "Build libmath tests" ON)
option(MATH_BUILD_EXAMPLES "Build libmath examples" ON)
option(MATH_BUILD_DOCS "Build libmath documentation" OFF)
option(MATH_INSTALL "Generate target for installing libmath" ON)
set_if_undefined(MATH_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/libmath" CACHE STRING
    "Install path for libmath package-related CMake files")

if(DEFINED MATH_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${MATH_SHARED_LIBS})
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(libmath) # initialized below
add_library(libmath::libmath ALIAS libmath)

#----------------------------------------------------------------------------------------------------------------------
# libmath dependencies
#----------------------------------------------------------------------------------------------------------------------

include(GoogleTest)

find_package(OpenMP)
if (OpenMP_CXX_FOUND)
    target_link_libraries(libmath OpenMP::OpenMP_CXX)
endif()

#----------------------------------------------------------------------------------------------------------------------
# libmath sources
#----------------------------------------------------------------------------------------------------------------------

include(GenerateExportHeader)
set(export_file_name "export_shared.h")

if(NOT BUILD_SHARED_LIBS)
    set(export_file_name "export_static.h")
endif()

generate_export_header(libmath EXPORT_FILE_NAME include/libmath/${export_file_name})

set(sources
    libmath/math_exception.h
    libmath/math_settings.h
    libmath/math_settings.cpp

    libmath/matrix.h

    libmath/boolean.h

    libmath/differential.h

    libmath/solver/las/lassolver.h
    libmath/solver/las/bicgstab.h
    libmath/solver/las/kholetsky.h
    libmath/solver/us/unlinearsolver.h
    libmath/solver/us/secant.h

    libmath/interpolator/interpolator.h
    libmath/interpolator/linear.h
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

#----------------------------------------------------------------------------------------------------------------------
# libmath target
#----------------------------------------------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

target_sources(libmath PRIVATE ${sources})
target_compile_definitions(libmath 
    PUBLIC 
        "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:MATH_STATIC_DEFINE>"
        "$<$<BOOL:${MATH_USE_DOUBLE_PRECISION}>:MATH_DOUBLE_PRECISION_DEFINE>")

target_include_directories(libmath
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

set_target_properties(libmath PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION})

if(MATH_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    configure_package_config_file(cmake/libmath-config.cmake.in libmath-config.cmake
        INSTALL_DESTINATION "${MATH_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(libmath-config-version.cmake
        COMPATIBILITY SameMajorVersion)

    install(TARGETS libmath EXPORT libmath_export
        RUNTIME COMPONENT libmath
        LIBRARY COMPONENT libmath NAMELINK_COMPONENT libmath-dev
        ARCHIVE COMPONENT libmath-dev
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(DIRECTORY libmath
        TYPE INCLUDE
        COMPONENT libmath-dev
        FILES_MATCHING PATTERN "*.h")
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/libmath/${export_file_name}"
        COMPONENT libmath-dev
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libmath")

    set(targets_file "libmath-shared-targets.cmake")

    if(NOT BUILD_SHARED_LIBS)
        set(targets_file "libmath-static-targets.cmake")
    endif()

    install(EXPORT libmath_export
        COMPONENT libmath-dev
        FILE "${targets_file}"
        DESTINATION "${MATH_INSTALL_CMAKEDIR}"
        NAMESPACE libmath::)

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/libmath-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/libmath-config-version.cmake"
        COMPONENT libmath-dev
        DESTINATION "${MATH_INSTALL_CMAKEDIR}")

    if(MSVC)
        set(pdb_file "")
        set(pdb_file_destination "")

        if(BUILD_SHARED_LIBS)
            set(pdb_file "$<TARGET_PDB_FILE:libmath>")
            set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
        else()
            # TARGET_PDB_FILE does not work for pdb file generated for static library build, determining it manually
            set(pdb_file "$<TARGET_FILE_DIR:libmath>/$<TARGET_FILE_PREFIX:libmath>$<TARGET_FILE_BASE_NAME:libmath>.pdb")
            set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
        endif()

        install(FILES "${pdb_file}"
            COMPONENT libmath-dev
            CONFIGURATIONS Debug RelWithDebInfo
            DESTINATION "${pdb_file_destination}"
            OPTIONAL)
    endif()
endif()

#----------------------------------------------------------------------------------------------------------------------
# other targets
#----------------------------------------------------------------------------------------------------------------------

if(MATH_BUILD_TESTS)
    enable_testing()
    set(gtest_force_shared_crt ON
        CACHE BOOL
        "Use shared (DLL) run-time lib even when Google Test is built as static lib."
        FORCE
    )
    find_package(GTest QUIET)
    if(GTest_FOUND)
        message("-- Found GTest")
    else()
        message("-- GTest not found, use local version of GTest")
        add_subdirectory(libs/googletest)
    endif()
    add_subdirectory(tests)
    

    # module tests
    
    #---------------------------------------------
    # matrix test
    #---------------------------------------------
    add_executable ( libmath-matrix-test )    
    if(GTest_FOUND)
        target_link_libraries( libmath-matrix-test
           PRIVATE
               libmath::libmath
               GTest::GTest
               GTest::Main
        )
    else()
        target_link_libraries( libmath-matrix-test
           PRIVATE
               libmath::libmath
               gtest
               gtest_main
        )
    endif()
    set(MatrixTestSources
        libmath/matrix.test.cpp
    )
    target_sources ( libmath-matrix-test PRIVATE ${MatrixTestSources} )

        
    #---------------------------------------------
    # LAS solver test
    #---------------------------------------------
    add_executable ( libmath-lassolver-test )    
    if(GTest_FOUND)
        target_link_libraries( libmath-lassolver-test
           PRIVATE
               libmath::libmath
               GTest::GTest
               GTest::Main
        )
    else()
        target_link_libraries( libmath-lassolver-test
           PRIVATE
               libmath::libmath
               gtest
               gtest_main
        )
    endif()
    set(LASTestSources
        libmath/solver/las/lassolver.test.cpp
    )
    target_sources ( libmath-lassolver-test PRIVATE ${LASTestSources} )

        
    #---------------------------------------------
    # differential operations test
    #---------------------------------------------
    add_executable ( libmath-differential-test )    
    if(GTest_FOUND)
        target_link_libraries( libmath-differential-test
           PRIVATE
               libmath::libmath
               GTest::GTest
               GTest::Main
        )
    else()
        target_link_libraries( libmath-differential-test
           PRIVATE
               libmath::libmath
               gtest
               gtest_main
        )
    endif()
    set(DiffTestSources
        libmath/differential.test.cpp
    )
    target_sources ( libmath-differential-test PRIVATE ${DiffTestSources} )

        
    #---------------------------------------------
    # unlinear solvers test
    #---------------------------------------------
    add_executable ( libmath-us-test )    
    if(GTest_FOUND)
        target_link_libraries( libmath-us-test
           PRIVATE
               libmath::libmath
               GTest::GTest
               GTest::Main
        )
    else()
        target_link_libraries( libmath-us-test
           PRIVATE
               libmath::libmath
               gtest
               gtest_main
        )
    endif()
    set(USTestSources
        libmath/solver/us/unlinearsolver.test.cpp
    )
    target_sources ( libmath-us-test PRIVATE ${USTestSources} )

        
    #---------------------------------------------
    # interpolators test
    #---------------------------------------------
    add_executable ( libmath-interpolator-test )    
    if(GTest_FOUND)
        target_link_libraries( libmath-interpolator-test
           PRIVATE
               libmath::libmath
               GTest::GTest
               GTest::Main
        )
    else()
        target_link_libraries( libmath-interpolator-test
           PRIVATE
               libmath::libmath
               gtest
               gtest_main
        )
    endif()
    set(InterpTestSources
        libmath/interpolator/interpolator.test.cpp
    )
    target_sources ( libmath-interpolator-test PRIVATE ${InterpTestSources} )
endif()

if(MATH_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(MATH_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(docs include)
endif()
