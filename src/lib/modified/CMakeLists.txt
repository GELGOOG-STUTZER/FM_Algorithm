set(PJ_NAME modified)
project(${PJ_NAME})

file(GLOB_RECURSE SRC_FILES *.cc *.h)

add_library(${PJ_NAME} ${SRC_FILES})
target_compile_options(${PJ_NAME} PRIVATE  -O3)

target_include_directories(${PJ_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

set_target_properties(${PJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_DIR}/lib/${PJ_NAME})
