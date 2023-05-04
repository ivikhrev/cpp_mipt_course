file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(utils STATIC ${HEADERS} ${SOURCES})
target_include_directories(utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
# target_compile_definitions(utils
#     PRIVATE TEST_DATA_DIR="${TARGET_TEST_DATA_PATH}"
# )
