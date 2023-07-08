set(CMAKE_TOOLCHAIN_FILE /Users/songlim/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
cmake_minimum_required(VERSION 3.12)
project(SnakeGame)

# Set source files
file(GLOB SOURCES src/*.cpp)

# setup clang-tidy command from executable + options
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=cppcoreguidelines-*")

set(CMAKE_CXX_STANDARD 14)


# Set root directory explicitly
set(ROOT_DIR ${CMAKE_SOURCE_DIR})

# Set include directories
include_directories(
    ${ROOT_DIR}/include
    /opt/homebrew/include
)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add run command
add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SnakeGame
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Add lint command
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CLANG_TIDY_INCLUDE_PATHS "${ROOT_DIR}/include;/opt/homebrew/include")

# add_custom_target(lint
#     COMMAND ${CMAKE_CXX_CLANG_TIDY} ${SOURCES}
#     "-extra-arg-before=-I${CLANG_TIDY_INCLUDE_PATHS}"
#     COMMENT "Running clang-tidy"
# )
# Find TensorFlow package

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
configure_file(${ROOT_DIR}/assets/VT323-Regular.ttf ${ROOT_DIR}/build/assets/VT323-Regular.ttf COPYONLY)
