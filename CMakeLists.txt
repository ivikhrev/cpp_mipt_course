cmake_minimum_required(VERSION 3.11)

project(cpp)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

include(CMakeParseArguments)
include(CTest)
include(GoogleTest)

macro(add_target)
    set(oneValueArgs NAME MAIN TEST TEST_DATA_PATH)
    set(multiValueArgs SOURCES HEADERS DEPENDENCIES INCLUDE_DIRECTORIES)
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

    set(TEST_NAME ${TARGET_NAME}_test)
    add_executable(${TEST_NAME} ${TARGET_TEST} ${TARGET_SOURCES} ${TARGET_HEADERS})
    if(TARGET_INCLUDE_DIRECTORIES)
        target_include_directories(${TEST_NAME} PRIVATE ${TARGET_INCLUDE_DIRECTORIES})
    endif()
    target_link_libraries(${TEST_NAME} GTest::gtest GTest::gtest_main utils)
    gtest_discover_tests(${TEST_NAME}
        TEST_PREFIX ${TARGET_NAME}.
        PROPERTIES ENVIRONMENT
            TEST_DATA_PATH=${TARGET_TEST_DATA_PATH}
    )
    target_compile_definitions(${TEST_NAME}
        PRIVATE TEST_DATA_DIR="${TARGET_TEST_DATA_PATH}"
    )
endmacro()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(thirdparty/gtest)
add_subdirectory(common)
add_subdirectory(01_caches)
add_subdirectory(02_triangel_intersection_3d)
