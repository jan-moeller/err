cmake_minimum_required(VERSION 3.14)
project(err
        VERSION 1.0.0
        LANGUAGES CXX
)

#############################################################################################################
# Prevent in-source builds
#############################################################################################################
if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif ()

#############################################################################################################
# Dependencies
#############################################################################################################
include(cmake/CPM.cmake)

CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.11.0")
CPMAddPackage("gh:jan-moeller/ctrx@1.0.0")

#############################################################################################################
# Build configuration
#############################################################################################################
option(ERR_BUILD_TESTS "Enable building the err tests" OFF)

message(STATUS "------------------------------------------------------------------------------")
message(STATUS "    ${PROJECT_NAME} (${PROJECT_VERSION})")
message(STATUS "------------------------------------------------------------------------------")
message(STATUS "Build type:                  ${CMAKE_BUILD_TYPE}")
message(STATUS "Build unit tests:            ${ERR_BUILD_TESTS}")

#############################################################################################################
# Main library target
#############################################################################################################
add_library(${PROJECT_NAME} INTERFACE
        include/err/detail/all_types_same.hpp
        include/err/detail/apply_non_type_template_arg.hpp
        include/err/detail/error_impl.hpp
        include/err/detail/first_non_type_template_arg.hpp
        include/err/detail/forward_like.hpp
        include/err/detail/is_overlap.hpp
        include/err/detail/is_subset.hpp
        include/err/detail/join_arrays.hpp
        include/err/error.hpp
        include/err/overloaded.hpp
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
target_include_directories(
        ${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)
target_link_libraries(${PROJECT_NAME} INTERFACE ctrx::ctrx)

string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)
packageProject(
        NAME ${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        NAMESPACE ${PROJECT_NAME}
        BINARY_DIR ${PROJECT_BINARY_DIR}
        INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
        INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
        VERSION_HEADER "${VERSION_HEADER_LOCATION}"
        COMPATIBILITY SameMajorVersion
        DEPENDENCIES "ctrx 1.0.0"
)

#############################################################################################################
# Unit test targets
#############################################################################################################
if (${ERR_BUILD_TESTS})
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif ()