compile:
	cmake --preset conan-release
	cmake --build --preset conan-release

run:
	build/Release/SnakeGame