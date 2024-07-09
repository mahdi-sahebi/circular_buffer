set(sources test_circular_buffer.cpp)
set(test ${PROJECT_NAME}_test)

add_executable(${test} ${sources})

find_package(GTest REQUIRED)
target_include_directories(${test} PRIVATE ${GTEST_INCLUDE_DIR})
target_link_libraries(${test} GTest::gtest GTest::gtest_main)