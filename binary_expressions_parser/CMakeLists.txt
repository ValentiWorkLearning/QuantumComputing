cmake_minimum_required(VERSION 3.14)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

Set(Boost_INSTALL_DIR ${CONAN_BOOST_ROOT})
find_package(Boost 1.75 REQUIRED)

set (TARGET_NAME FirstTaskQuantumCoverage)
add_executable(${TARGET_NAME} ${PROJECT_SOURCES})

target_sources(
    ${TARGET_NAME}
    PRIVATE
    main.cpp
 "types.hpp" "parser.hpp" "debug_printer.hpp" "debug_tree_visualize.hpp" "evaluator.hpp")
target_compile_features(
    ${TARGET_NAME}
    PRIVATE
    cxx_std_20
)
target_compile_definitions(
    ${TARGET_NAME}
    PRIVATE
    NOMINMAX
    BUILD_EVALUATOR
)

target_include_directories(${TARGET_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE ${Boost_LIBRARIES})