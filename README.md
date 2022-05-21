# MHE-3-Part-problem

Three partition problem using metaheuristics algorithms.

to generate:
```
cmake -S "" -B "build" -G "Visual Studio 15 2017 Win64"
```

or

```
cmake -S "" -B "build" -G "Visual Studio 16 2019" -A "x64"
```

to build:
```
cmake --build "build"
```

Usage:
```
outfile.out [max_iterations] [input_file_path]
```

Example:
```
outfile.out 10000 testing_data.txt
```