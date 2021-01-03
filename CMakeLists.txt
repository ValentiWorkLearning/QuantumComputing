cmake_minimum_required(VERSION 3.14)

find_package(Boost 1.71)

set (TARGET_NAME FirstTaskQuantumCoverage)
add_executable(${TARGET_NAME} ${PROJECT_SOURCES})

target_sources(
    ${TARGET_NAME}
    PRIVATE
    main.cpp
 "types.hpp" "parser.hpp" "debug_priniter.hpp" "debug_tree_visualize.hpp")
target_compile_features(
    ${TARGET_NAME}
    PRIVATE
    cxx_std_20
)
target_include_directories(${TARGET_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE ${Boost_LIBRARIES})