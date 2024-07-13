set(sources circular_buffer.cpp)

find_package(Threads REQUIRED)
add_library(${PROJECT_NAME} STATIC ${sources})

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME}
    PUBLIC Threads::Threads)
