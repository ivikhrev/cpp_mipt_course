cmake_minimum_required(VERSION 3.11)
project(cpp)

include(CMakeParseArguments)

macro(add_target)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES HEADERS DEPENDENCIES INCLUDE_DIRECTORIES)
    cmake_parse_arguments(TARGET "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

    # Create executable file from sources
    add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${TARGET_HEADERS})

    if(TARGET_INCLUDE_DIRECTORIES)
        target_include_directories(${TARGET_NAME} PRIVATE ${TARGET_INCLUDE_DIRECTORIES})
    endif()

    if(TARGET_INCLUDE_DIRECTORIES)
        target_link_libraries(${TARGET_NAME} PRIVATE ${TARGET_DEPENDENCIES})
    endif()

endmacro()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(01_caches)