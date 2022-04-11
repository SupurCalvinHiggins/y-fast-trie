# y-fast-trie
Y-Fast Trie

A C++ implementation of a Y-Fast Trie.

## Overview
This library implements the following data structures:
    1. Y-Fast Trie
    2. X-Fast Trie
    3. Red-Black Tree

## Installation
To install dependencies on Linux systems, execute
```
sudo chmod +x install_dependencies.sh
sudo ./install_dependencies.sh
```

On MacOS and Windows, ensure that the following dependencies are installed:
    1. python3
    2. python3-pip
    3. GCC
    4. termcolor
    5. Google Test
    6. Google Benchmark

To ensure the library functions, execute
```
python3 run.py test
```

To ensure the benchmarks function, execute
```
python3 run.py benchmark
```

## Testing

The library implements a full test suite using the Google Test C++ framework.

### Executing Tests
The run.py Python script provides useful functionality for testing the library.

To run all tests, execute
```
python3 run.py test
```

To run tests in a directory, execute
```
python3 run.py <path>
```

To run a specific test, execute
```
python3 run.py <path>
```

To run multiple specific tests, execute
```
python3 run.py <path1> <path2> ...
```

### Tweaking Tests
Certain constants are set in test/base/constants.h. See the file for more details.

### Adding Tests
Test fixture implementations are stored in test/fixtures and come with several builtin tests. These can be accessed by including test/fixtures/fixtures.h and defining the appropriate test fixture. For example, to add insertion tests in test/my-data-structure write
```
#include "../fixtures/fixtures.h"
#include <my-data-structure>

DEFINE_INSERT_TESTS(MyDataStructure);
TEST_MAIN();
```
to test/my-data-structure/insert.test.cpp.

Tests can be slow when they are executed as seperate files. Place test.cpp in a testing directory to subsume other tests when the directory is selected for testing.

## Benchmarking
The library implements a full benchmarking suite using the Google Benchmark C++ framework.

### Executing Tests
The run.py Python script provides useful functionality for benchmarking the library.

To run all benchmarks, execute
```
python3 run.py benchmark
```

To run benchmarks in a directory, execute
```
python3 run.py <path>
```

To run a specific benchmark, execute
```
python3 run.py <path>
```

To run multiple specific benchmarks, execute
```
python3 run.py <path1> <path2> ...
```

### Tweaking Benchmarks
Certain constants are set in benchmark/base/constants.h. See the file for more details.

### Adding Benchmarks
Benchmark fixture implementations are stored in benchmark/fixtures and come with several builtin benchmarks. These can be accessed by including benchmark/fixtures/fixtures.h and defining the appropriate benchmark fixture. For example, to add insertion benchmarks in benchmark/my-data-structure write
```
#include "../fixtures/fixtures.h"
#include <my-data-structure>

DEFINE_INSERT_BENCHMARKS(MyDataStructure);
BENCHMARK_MAIN();
```
to benchmark/my-data-structure/insert.benchmark.cpp.

Benchmark subsumption is currently unsupported.