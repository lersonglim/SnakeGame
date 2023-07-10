# C++ Program Compilation Guide

This guide will walk you through the steps to compile and run a SnakeGame program using CMake.

## Prerequisites

Before you begin, make sure you have the following installed on your system:

- CMake
- C++ compiler (e.g., GCC or Clang)

## Compilation Steps

1. Open your terminal or command prompt.

2. Change the directory to the `build` folder:
   ```shell
   cd build
   ```
3. Run CMake to generate the build files:
   ```shell
   cmake ..
   ```

This command will generate the necessary build files based on the `CMakeLists.txt` file in the project root directory.

4. Run the make command to compile the C++ program:

   ```shell
   make
   ```

The make command will compile the source code and generate the executable.

5. Finally, run the program:

   ```shell
   ./bin/SnakeGame
   ```

This command executes the compiled program.
