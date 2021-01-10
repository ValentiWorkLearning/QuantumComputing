cmake_minimum_required(VERSION 3.14)

set (TARGET_NAME SecondFourthTaskSequentialLogic)
add_executable(${TARGET_NAME} ${PROJECT_SOURCES})

target_sources(
    ${TARGET_NAME}
    PRIVATE
    main.cpp)
target_compile_features(
    ${TARGET_NAME}
    PRIVATE
    cxx_std_20
)
target_compile_definitions(
    ${TARGET_NAME}
    PRIVATE
    NOMINMAX
    USE_ARGV_FILE_PATH
)