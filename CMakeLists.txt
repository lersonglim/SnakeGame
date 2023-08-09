cmake_minimum_required(VERSION 3.15)
project(SnakeGame CXX)

# Set include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)
find_package(boost REQUIRED)
find_package(nlohmann_json REQUIRED)

# Build executable
file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# Main SDL library for init
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2-static)
target_link_libraries(${PROJECT_NAME} PRIVATE boost::boost)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# SDL ttf so we can display hello world!
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf)

# Copy font file to build directory
configure_file(${CMAKE_SOURCE_DIR}/assets/VT323-Regular.ttf ${CMAKE_SOURCE_DIR}/build/assets/VT323-Regular.ttf COPYONLY)