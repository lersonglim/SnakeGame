cmake_minimum_required(VERSION 3.12)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 14)

# Set root directory explicitly
set(ROOT_DIR ${CMAKE_SOURCE_DIR})

# Set include directories
include_directories(include)
include_directories(/opt/homebrew/include)

# Set source files
file(GLOB SOURCES src/*.cpp)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Build executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${CMAKE_DL_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    "-L/opt/homebrew/lib"
    -lSDL2
    -lSDL2_ttf
)

# Copy font file to build directory
configure_file(${ROOT_DIR}/assets/debrosee.ttf ${ROOT_DIR}/build/assets/debrosee.ttf COPYONLY)
