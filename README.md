to generate:
cmake -S "" -B "build" -G "Visual Studio 15 2017 Win64"
    or
cmake -S "" -B "build" -G "Visual Studio 16 2019" -A "x64"

to build:
cmake --build "build"