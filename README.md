# C++ Program Compilation Guide

This guide will walk you through the steps to compile and run a C++ program using CMake.

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
   ./bin/program
   ```

This command executes the compiled program.

Congratulations! You have successfully compiled and run the C++ program using CMake.

## Troubleshooting

- If you encounter any errors during the compilation process, make sure you have installed the required dependencies and that the project's `CMakeLists.txt` file is correctly configured.

- If the program fails to run, ensure that the compiled executable is located in the correct directory, as specified in the last step.

## Additional Resources

For more information on using CMake and building C++ projects, refer to the following resources:

- [CMake Documentation](https://cmake.org/documentation/)
- [C++ Reference](http://www.cplusplus.com/doc/)
