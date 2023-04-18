cmake_minimum_required(VERSION 3.11)

project(cpp)

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

include(CMakeParseArguments)
include(GoogleTest)

macro(add_target)
    set(oneValueArgs NAME)
    set(multiValueArgs MAIN TEST SOURCES HEADERS DEPENDENCIES INCLUDE_DIRECTORIES)
    cmake_parse_arguments(TARGET "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

    # Create executable file from sources
    add_executable(${TARGET_NAME} ${TARGET_MAIN} ${TARGET_SOURCES} ${TARGET_HEADERS})

    if(TARGET_INCLUDE_DIRECTORIES)
        target_include_directories(${TARGET_NAME} PRIVATE ${TARGET_INCLUDE_DIRECTORIES})
    endif()

    if(TARGET_DEPENDENCIES)
        target_link_libraries(${TARGET_NAME} PRIVATE ${TARGET_DEPENDENCIES})
    endif()

    add_executable(${TARGET_NAME}_tests ${TARGET_TEST} ${TARGET_SOURCES} ${TARGET_HEADERS})
    if(TARGET_INCLUDE_DIRECTORIES)
        target_include_directories(${TARGET_NAME}_tests PRIVATE ${TARGET_INCLUDE_DIRECTORIES})
    endif()
    target_link_libraries(${TARGET_NAME}_tests GTest::gtest_main)
    gtest_discover_tests(${TARGET_NAME}_tests)
endmacro()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(thirdparty/gtest)

enable_testing()
add_subdirectory(01_caches)
add_subdirectory(02_triangel_intersection_3d)