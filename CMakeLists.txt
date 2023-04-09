cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME FM_Algorithm)
project(${PROJECT_NAME})

file(GLOB_RECURSE SRC_FILES src/lib/*.cc)
add_executable(${PROJECT_NAME} src/main.cc ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
target_compile_options(${PROJECT_NAME} PRIVATE  -O3)

set(OUT_DIR ${CMAKE_BINARY_DIR}/bin/)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})

add_subdirectory(src/lib)
target_link_libraries(${PROJECT_NAME} modified)
target_link_libraries(${PROJECT_NAME} original)
