
cmake_minimum_required( VERSION 3.2 ) 
set( CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Supported configuration types" FORCE )
        
if( NOT ROOT_DIRECTORY )
    set( ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
    if ( NOT fetched_subprojects )
        if ( NOT PYTHON_EXECUTABLE )
            find_package( PythonInterp )
            if ( NOT PYTHONINTERP_FOUND )
                message( FATAL_ERROR "Python interpeter installation was not found." )
            endif()
        endif()
        execute_process( COMMAND ${PYTHON_EXECUTABLE} "./metaconfigure/fetch_subprojects.py"
                         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
                         RESULT_VARIABLE fetch_failure )
        if ( NOT fetch_failure )
            set( fetched_subprojects TRUE CACHE BOOL "fetch script ran" )
        else()
            message( FATAL_ERROR "Failed to fetch dependencies" )
        endif()
    endif()
endif()

project( interpolation LANGUAGES CXX )
get_directory_property( is_subproject PARENT_DIRECTORY )
include( CMakeDependentOption REQUIRED )

set( interpolation_GNU_minimum_version 6.2 )
set( interpolation_Clang_minimum_version 3.8 )
set( interpolation_AppleClang_minimum_version 8.0 )

if( interpolation_${CMAKE_CXX_COMPILER_ID}_minimum_version )
    if( CMAKE_CXX_COMPILER_VERSION AND
        CMAKE_CXX_COMPILER_VERSION VERSION_LESS
        ${interpolation_${CMAKE_CXX_COMPILER_ID}_minimum_version} )
        message( FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} version must be greater than ${interpolation_${CMAKE_CXX_COMPILER_ID}_minimum_version}" )
    endif()
endif()

# general properties
option( strict "Compile time warnings are converted to errors" ON )

# binary instrumentation
option( coverage "Enable binary instrumentation to collect test coverage information in the DEBUG configuration" )
option( profile_generate "Enable binary instrumentation to generation execution profiles in the RELEASE configuration which may be used to guide later optimization" )

# additional optimizations
option( link_time_optimization "Enable link time optimization in the RELEASE configuration" )
option( profile_use "In the RELEASE configuration, leverage previously generated exeution profile to inform optimization decisions" )
option( nonportable_optimization "Enable optimizations which compromise portability of resulting binary in the RELEASE configuration" )

# libraries and linking
option( static "Statically link component and environment libraries" OFF )
if ( static AND ( "${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin" ) )
    message( FATAL_ERROR "Static binaries not supported on OSX" )
endif()

CMAKE_DEPENDENT_OPTION( static_libraries "Statically link component libraries" OFF "NOT static" ON )

option( unit_tests "Compile the interpolation unit tests and integrate with ctest" ON ) 

if ( profile_generate AND profile_use )
    message( FATAL_ERROR "Cannot both generate and use execution profile in the same configuration" )
endif()

if ( profile_generate )
    file( MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/profiling" )
endif()

set( interpolation_GNU_Windows_common_flags "-Wall" "-Wextra" "-Wpedantic" "-std=c++14" )
set( interpolation_GNU_Windows_DEBUG_flags "-O0" "-g" "-gdwarf-3" "-fsignaling-nans" )
set( interpolation_GNU_Windows_RELEASE_flags "-O3" "-DNDEBUG" )
set( interpolation_GNU_Windows_strict_flags "-Werror" )
set( interpolation_GNU_Windows_coverage_flags "--coverage" )
set( interpolation_GNU_Windows_subproject_flags  )
set( interpolation_GNU_Windows_base_project_flags  )
set( interpolation_GNU_Windows_profile_generate_flags "-fprofile-generate='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_GNU_Windows_link_time_optimization_flags "-flto" )
set( interpolation_GNU_Windows_profile_use_flags "-fprofile-use='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_GNU_Windows_nonportable_optimization_flags "-march=native" )
set( interpolation_GNU_Windows_static_flags "-static" )
set( interpolation_GNU_Linux_common_flags "-Wall" "-Wextra" "-Wpedantic" "-std=c++14" )
set( interpolation_GNU_Linux_DEBUG_flags "-O0" "-g" "-gdwarf-3" "-fsignaling-nans" )
set( interpolation_GNU_Linux_RELEASE_flags "-O3" "-DNDEBUG" )
set( interpolation_GNU_Linux_strict_flags "-Werror" )
set( interpolation_GNU_Linux_coverage_flags "--coverage" )
set( interpolation_GNU_Linux_subproject_flags  )
set( interpolation_GNU_Linux_base_project_flags  )
set( interpolation_GNU_Linux_profile_generate_flags "-fprofile-generate='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_GNU_Linux_link_time_optimization_flags "-flto" )
set( interpolation_GNU_Linux_profile_use_flags "-fprofile-use='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_GNU_Linux_nonportable_optimization_flags "-march=native" )
set( interpolation_GNU_Linux_static_flags "-static" )
set( interpolation_GNU_Darwin_common_flags "-Wall" "-Wextra" "-Wpedantic" "-std=c++14" )
set( interpolation_GNU_Darwin_DEBUG_flags "-O0" "-g" "-gdwarf-3" "-fsignaling-nans" )
set( interpolation_GNU_Darwin_RELEASE_flags "-O3" "-DNDEBUG" )
set( interpolation_GNU_Darwin_strict_flags "-Werror" )
set( interpolation_GNU_Darwin_coverage_flags "--coverage" )
set( interpolation_GNU_Darwin_subproject_flags  )
set( interpolation_GNU_Darwin_base_project_flags  )
set( interpolation_GNU_Darwin_profile_generate_flags "-fprofile-generate='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_GNU_Darwin_link_time_optimization_flags "-flto" )
set( interpolation_GNU_Darwin_profile_use_flags "-fprofile-use='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_GNU_Darwin_nonportable_optimization_flags "-march=native" )
set( interpolation_GNU_Darwin_static_flags "-static" )
set( interpolation_Clang_Windows_common_flags "-stdlib=libc++" "-Wall" "-Wextra" "-Wpedantic" "-std=c++14" )
set( interpolation_Clang_Windows_DEBUG_flags "-O0" "-g" "-gdwarf-3" )
set( interpolation_Clang_Windows_RELEASE_flags "-O3" "-DNDEBUG" )
set( interpolation_Clang_Windows_strict_flags "-Werror" )
set( interpolation_Clang_Windows_coverage_flags "--coverage" )
set( interpolation_Clang_Windows_subproject_flags  )
set( interpolation_Clang_Windows_base_project_flags  )
set( interpolation_Clang_Windows_profile_generate_flags "-fprofile-generate='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_Clang_Windows_link_time_optimization_flags "-flto" )
set( interpolation_Clang_Windows_profile_use_flags "-fprofile-use='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_Clang_Windows_nonportable_optimization_flags "-march=native" )
set( interpolation_Clang_Windows_static_flags "-static" )
set( interpolation_Clang_Linux_common_flags "-stdlib=libstdc++" "-Wall" "-Wextra" "-Wpedantic" "-std=c++14" )
set( interpolation_Clang_Linux_DEBUG_flags "-O0" "-g" "-gdwarf-3" )
set( interpolation_Clang_Linux_RELEASE_flags "-O3" "-DNDEBUG" )
set( interpolation_Clang_Linux_strict_flags "-Werror" )
set( interpolation_Clang_Linux_coverage_flags "--coverage" )
set( interpolation_Clang_Linux_subproject_flags  )
set( interpolation_Clang_Linux_base_project_flags  )
set( interpolation_Clang_Linux_profile_generate_flags "-fprofile-generate='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_Clang_Linux_link_time_optimization_flags "-flto" )
set( interpolation_Clang_Linux_profile_use_flags "-fprofile-use='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_Clang_Linux_nonportable_optimization_flags "-march=native" )
set( interpolation_Clang_Linux_static_flags "-static" )
set( interpolation_Clang_Darwin_common_flags "-stdlib=libc++" "-Wall" "-Wextra" "-Wpedantic" "-std=c++14" )
set( interpolation_Clang_Darwin_DEBUG_flags "-O0" "-g" "-gdwarf-3" )
set( interpolation_Clang_Darwin_RELEASE_flags "-O3" "-DNDEBUG" )
set( interpolation_Clang_Darwin_strict_flags "-Werror" )
set( interpolation_Clang_Darwin_coverage_flags "--coverage" )
set( interpolation_Clang_Darwin_subproject_flags  )
set( interpolation_Clang_Darwin_base_project_flags  )
set( interpolation_Clang_Darwin_profile_generate_flags "-fprofile-generate='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_Clang_Darwin_link_time_optimization_flags "-flto" )
set( interpolation_Clang_Darwin_profile_use_flags "-fprofile-use='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_Clang_Darwin_nonportable_optimization_flags "-march=native" )
set( interpolation_Clang_Darwin_static_flags "-static" )
set( interpolation_AppleClang_Darwin_common_flags "-stdlib=libc++" "-Wall" "-Wextra" "-Wpedantic" "-std=c++14" )
set( interpolation_AppleClang_Darwin_DEBUG_flags "-O0" "-g" "-gdwarf-3" )
set( interpolation_AppleClang_Darwin_RELEASE_flags "-O3" "-DNDEBUG" )
set( interpolation_AppleClang_Darwin_strict_flags "-Werror" )
set( interpolation_AppleClang_Darwin_coverage_flags "--coverage" )
set( interpolation_AppleClang_Darwin_subproject_flags  )
set( interpolation_AppleClang_Darwin_base_project_flags  )
set( interpolation_AppleClang_Darwin_profile_generate_flags "-fprofile-generate='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_AppleClang_Darwin_link_time_optimization_flags "-flto" )
set( interpolation_AppleClang_Darwin_profile_use_flags "-fprofile-use='${CMAKE_BINARY_DIR}/profiling'" )
set( interpolation_AppleClang_Darwin_nonportable_optimization_flags "-march=native" )
set( interpolation_AppleClang_Darwin_static_flags "-static" )
get_directory_property( is_subproject PARENT_DIRECTORY )

if( NOT TARGET variant-adapter )
    add_subdirectory( ${ROOT_DIRECTORY}/subprojects/variant-adapter )
endif()

if( NOT TARGET hana-adapter )
    add_subdirectory( ${ROOT_DIRECTORY}/subprojects/hana-adapter )
endif()

if( NOT TARGET range-v3-adapter )
    add_subdirectory( ${ROOT_DIRECTORY}/subprojects/range-v3-adapter )
endif()

if( NOT TARGET catch-adapter )
    add_subdirectory( ${ROOT_DIRECTORY}/subprojects/catch-adapter )
endif()

if( NOT TARGET dimwits )
    add_subdirectory( ${ROOT_DIRECTORY}/subprojects/dimwits )
endif()

if( NOT TARGET fmt-adapter )
    add_subdirectory( ${ROOT_DIRECTORY}/subprojects/fmt-adapter )
endif()

if( NOT TARGET spdlog-adapter )
    add_subdirectory( ${ROOT_DIRECTORY}/subprojects/spdlog-adapter )
endif()

if( NOT TARGET Log )
    add_subdirectory( ${ROOT_DIRECTORY}/subprojects/Log )
endif()

if( NOT TARGET header-utilities )
    add_subdirectory( ${ROOT_DIRECTORY}/subprojects/header-utilities )
endif()


if ( NOT GIT_EXECUTABLE )
    find_package( Git )
    if ( NOT GIT_FOUND )
        message( FATAL_ERROR "git installation was not found." )
    endif()
endif()

execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
) 

message( STATUS "" )
message( STATUS "-----------------------------------------------------------" )
message( STATUS "" )
message( STATUS "interpolation" )
message( STATUS "Git current branch: ${GIT_BRANCH}" )
message( STATUS "Git commit hash: ${GIT_HASH}" )
message( STATUS "" )
message( STATUS "-----------------------------------------------------------" )

add_library( interpolation INTERFACE )
target_sources( interpolation INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/Interpolant.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/Tags.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/Interpolant/Histogram.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/Interpolant/LinearLinear.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/Interpolant/LinearLogarithmic.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/Interpolant/LogarithmicLinear.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/Interpolant/LogarithmicLogarithmic.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/discontinuity.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/domain.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Implementation.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/left.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Make.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/right.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/search.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Table.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/transform.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Type.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Variant.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Vector.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/domain/Max.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/domain/Min.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/domain/max/IsCompiletimeConstant.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/domain/max/IsRuntimeConstant.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/domain/min/IsCompiletimeConstant.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/domain/min/IsRuntimeConstant.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/left/interval.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/left/interval/IsCompiletimeConstant.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/left/interval/IsRuntimeConstant.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/left/interval/IsRuntimeValue.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/left/interval/Throws.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/right/interval.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/right/interval/IsCompiletimeConstant.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/right/interval/IsRuntimeConstant.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/right/interval/IsRuntimeValue.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/right/interval/Throws.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/search/Binary.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/search/Cached.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/search/FromLeft.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/search/FromRight.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/search/Hashed.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Type/Make.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Type/Table.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Type/src/ctor.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Type/src/evaluate.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Variant/helper.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Variant/Iterator.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Variant/Search.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Variant/Table.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Vector/Table.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Vector/src/ctor.hpp"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation/table/Vector/src/evaluate.hpp" )

target_include_directories( interpolation INTERFACE src )

set( PREFIX interpolation_${CMAKE_CXX_COMPILER_ID}_${CMAKE_SYSTEM_NAME} )

target_link_libraries( interpolation INTERFACE "$<$<AND:$<CONFIG:RELEASE>,$<BOOL:${link_time_optimization}>>:${${PREFIX}_RELEASE_flags};${${PREFIX}_link_time_optimization_flags}$<$<BOOL:${profile_generate}>:${${PREFIX}_profile_generate_flags};>$<$<BOOL:${profile_use}>:${${PREFIX}_profile_use_flags};>$<$<BOOL:${nonportable_optimization}>:${${PREFIX}_nonportable_optimization_flags};>>$<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${${PREFIX}_coverage_flags};>>$<$<BOOL:CXX_appended_flags>:${CXX_appended_flags};>$<$<BOOL:interpolation_appended_flags>:${interpolation_appended_flags};>" )

target_link_libraries( interpolation INTERFACE dimwits INTERFACE range-v3-adapter INTERFACE variant-adapter INTERFACE header-utilities INTERFACE Log )

if( NOT is_subproject )
    enable_testing() 
    if ( unit_tests )
        add_subdirectory( src/interpolation/Interpolant/LogarithmicLogarithmic/test )
        add_subdirectory( src/interpolation/table/search/test )
        add_subdirectory( src/interpolation/Interpolant/Histogram/test )
        add_subdirectory( src/interpolation/table/left/interval/IsCompiletimeConstant/test )
        add_subdirectory( src/interpolation/table/left/interval/Throws/test )
        add_subdirectory( src/interpolation/Interpolant/LogarithmicLinear/test )
        add_subdirectory( src/interpolation/table/transform/test )
        add_subdirectory( src/interpolation/table/Variant/test )
        add_subdirectory( src/interpolation/table/right/interval/Throws/test )
        add_subdirectory( src/interpolation/table/left/interval/IsRuntimeConstant/test )
        add_subdirectory( src/interpolation/table/right/interval/IsRuntimeConstant/test )
        add_subdirectory( src/interpolation/table/domain/max/IsRuntimeConstant/test )
        add_subdirectory( src/interpolation/test )
        add_subdirectory( src/interpolation/table/Variant/Iterator/helper/test )
        add_subdirectory( src/interpolation/Interpolant/LinearLinear/test )
        add_subdirectory( src/interpolation/table/Vector/test )
        add_subdirectory( src/interpolation/Interpolant/LinearLogarithmic/test )
        add_subdirectory( src/interpolation/table/domain/min/IsRuntimeConstant/test )
        add_subdirectory( src/interpolation/table/Variant/Iterator/test )
        add_subdirectory( src/interpolation/table/right/interval/IsCompiletimeConstant/test )
        add_subdirectory( src/interpolation/table/Type/test )
        add_subdirectory( src/interpolation/table/make/test )
        add_subdirectory( src/interpolation/table/Table/test )
        add_subdirectory( src/interpolation/table/domain/min/IsCompiletimeConstant/test )
        add_subdirectory( src/interpolation/table/domain/max/IsCompiletimeConstant/test )
    endif() 
endif()

install( DIRECTORY src/ DESTINATION include
         FILE_PERMISSIONS OWNER_READ OWNER_WRITE 
                          GROUP_READ 
                          WORLD_READ
         DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE 
                               GROUP_READ 
                               WORLD_READ
         FILES_MATCHING REGEX ".*\.h[+][+]$|.*\.hxx$|.*\.hpp$|.*\.hh$|.*\.h$"  )
