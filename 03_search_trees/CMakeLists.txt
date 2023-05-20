file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_target(NAME search_trees
           MAIN "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
           TEST "${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp"
           SOURCES ${SOURCES}
           HEADERS ${HEADERS}
           INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
           TEST_DATA_GENERATOR "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_data_generator.py"
           GEN_CMD "-n" "10"
               "--elems_num" "10000"
                "-d" "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_data"
           TEST_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_data")
