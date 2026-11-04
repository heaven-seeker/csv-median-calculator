cmake_minimum_required(VERSION 3.23)
project(csv_median_calculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS program_options)

include(FetchContent)

FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(spdlog)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/config_parser.cpp
        src/file_scanner.cpp
        src/csv_reader.cpp
        src/median_calculator.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::program_options
    tomlplusplus::tomlplusplus
    spdlog::spdlog
)