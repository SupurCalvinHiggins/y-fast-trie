# y-fast-trie
Y-Fast Trie

A C++ implementation of a Y-Fast Trie. 

## Preamble

**Do not use in production code!!!** 

Check out https://github.com/pdinklag/tdc/tree/sea21-predecessor and the paper "Engineering Predecessor Data Structures for
Dynamic Integer Sets" for a quality implementation instead.

## Overview
This library implements the following data structures:  
1. Y-Fast Trie  
2. X-Fast Trie  
3. Red-Black Tree  

## Installation

### C++ Version

The library requires at least C++17. With some tweaks, the library will work for lower C++ version given a custom std::optional implementation.

### Linux
To install dependencies on Linux systems, execute
```
sudo chmod +x install_dependencies.sh
sudo ./install_dependencies.sh
```

### MacOS and Windows
On MacOS and Windows, ensure that the following dependencies are installed:
1. python3
2. python3-pip
3. GCC
4. termcolor
5. Google Test
6. Google Benchmark

### Verification
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
```c++
#include "../fixtures/fixtures.h"
#include <my-data-structure>

DEFINE_INSERT_TESTS(MyDataStructure);
TEST_MAIN();
```
to test/my-data-structure/insert.test.cpp.

Tests can be slow when they are executed as seperate files. Place test.cpp in a testing directory to subsume other tests when the directory is selected for testing.

## Benchmarking
The library implements a full benchmarking suite using the Google Benchmark C++ framework.

### Executing Benchmarks
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
```c++
#include "../fixtures/fixtures.h"
#include <my-data-structure>

DEFINE_INSERT_BENCHMARKS(MyDataStructure);
BENCHMARK_MAIN();
```
to benchmark/my-data-structure/insert.benchmark.cpp.

Benchmark subsumption is currently unsupported.


## Red-Black Tree
Red-Black Trees are balanced binary search trees that provide dynamic ordered set operations in O(log(N)) time with O(N) space.

### Interface
The Red-Black Tree provides the following interface:
```c++
void insert(key_type key);
void remove(key_type key);

some_key_type predecessor(key_type key);
some_key_type successor(key_type key);

some_key_type min(key_type key);
some_key_type max(key_type key);

bool contains(key_type key);

key_type lower_bound();
key_type upper_bound();
```

Double insertions and double removes are ignored.

### Complexity
The Red-Black Tree methods run in the following time complexities where N is the number of keys.

| Operation | Time complexity |
| --- | --- |
| insert | O(log(N)) |
| remove | O(log(N)) |
| predecessor | O(log(N)) |
| successor | O(log(N)) |
| min | O(log(N)) |
| max | O(log(N)) |
| contains | O(1) |
| lower_bound | O(1) |
| upper_bound | O(1) |

### Example

Sample Program:
```c++
#include "../src/red-black-tree/red-black-tree.h"
#include <iostream>
#include <cstdint>

int main() {
    RedBlackTree<uint32_t> tree;

    tree.insert(5);
    if (tree.contains(5))
        std::cout << "The tree contains the number 5." << std::endl;

    tree.insert(5);
    if (tree.contains(5))
        std::cout << "Double insertions are effectively no-ops." << std::endl;
    
    tree.remove(5);
    if (!tree.contains(5))
        std::cout << "The tree does not contain the number 5." << std::endl;
    
    tree.remove(5);
    if (!tree.contains(5))
        std::cout << "Double removes are effectively no-ops." << std::endl;
    
    tree.insert(7);
    tree.insert(11);
    tree.insert(16);

    std::cout << "The successor of 11 is "  << tree.successor(11).value() << "." << std::endl;
    std::cout << "The predecessor of 11 is "  << tree.predecessor(11).value() << "." << std::endl;

    std::cout << "The successor of 10 is "  << tree.successor(10).value() << "." << std::endl;
    std::cout << "The predecessor of 10 is "  << tree.predecessor(10).value() << "." << std::endl;

    std::cout << "Does the successor of 16 exist? "  << tree.successor(16).has_value() << "." << std::endl;
    std::cout << "Does the predecessor of 7 exist? "  << tree.predecessor(7).has_value() << "." << std::endl;
}
```

Sample Output:
```
The trie contains the number 5.
Double insertions are effectively no-ops.
The trie does not contain the number 5.
Double removes are effectively no-ops.
The successor of 11 is 16.
The predecessor of 11 is 7.
The successor of 10 is 11.
The predecessor of 10 is 7.
Does the successor of 16 exist? 0.
Does the predecessor of 7 exist? 0.
```

## X-Fast Trie
X-Fast Tries are bitwise tries that implement ordered set operations in O(log(log(M))) time and dynamic set operations in O(log(M)) time in O(N) space.

### Interface
The X-Fast Trie provides the following interface:
```c++
void insert(key_type key);
void remove(key_type key);

some_key_type predecessor(key_type key);
some_key_type successor(key_type key);

some_key_type min(key_type key);
some_key_type max(key_type key);

bool contains(key_type key);

key_type lower_bound();
key_type upper_bound();
```

Double insertions and double removes are ignored.


### Complexity
The X-Fast Trie methods run in the following time complexities where M is the size of universe.

| Operation | Time complexity |
| --- | --- |
| insert | O(log(M)) |
| remove | O(log(M)) |
| predecessor | O(log(log(M))) |
| successor | O(log(log(M))) |
| min | O(log(log(M))) |
| max | O(log(log(M))) |
| contains | O(1) |
| lower_bound | O(1) |
| upper_bound | O(1) |

### Example

Sample Program:
```c++
#include "src/x-fast-trie/x-fast-trie.h"
#include <iostream>
#include <cstdint>

int main() {
    XFastTrie<uint32_t> trie;

    trie.insert(5);
    if (trie.contains(5))
        std::cout << "The trie contains the number 5." << std::endl;

    trie.insert(5);
    if (trie.contains(5))
        std::cout << "Double insertions are effectively no-ops." << std::endl;
    
    trie.remove(5);
    if (!trie.contains(5))
        std::cout << "The trie does not contain the number 5." << std::endl;
    
    trie.remove(5);
    if (!trie.contains(5))
        std::cout << "Double removes are effectively no-ops." << std::endl;
    
    trie.insert(7);
    trie.insert(11);
    trie.insert(16);

    std::cout << "The successor of 11 is "  << trie.successor(11).value() << "." << std::endl;
    std::cout << "The predecessor of 11 is "  << trie.predecessor(11).value() << "." << std::endl;

    std::cout << "The successor of 10 is "  << trie.successor(10).value() << "." << std::endl;
    std::cout << "The predecessor of 10 is "  << trie.predecessor(10).value() << "." << std::endl;

    std::cout << "Does the successor of 16 exist? "  << trie.successor(16).has_value() << "." << std::endl;
    std::cout << "Does the predecessor of 7 exist? "  << trie.predecessor(7).has_value() << "." << std::endl;
}
```

Sample Output:
```
The trie contains the number 5.
Double insertions are effectively no-ops.
The trie does not contain the number 5.
Double removes are effectively no-ops.
The successor of 11 is 16.
The predecessor of 11 is 7.
The successor of 10 is 11.
The predecessor of 10 is 7.
Does the successor of 16 exist? 0.
Does the predecessor of 7 exist? 0.
```

## Y-Fast Trie
Y-Fast Tries are bitwise tries that implement dynamic ordered set operations in O(log(log(M))) time in O(N) space.

### Interface
The Y-Fast Trie provides the following interface:
```c++
void insert(key_type key);
void remove(key_type key);

some_key_type predecessor(key_type key);
some_key_type successor(key_type key);

some_key_type min(key_type key);
some_key_type max(key_type key);

bool contains(key_type key);

key_type lower_bound();
key_type upper_bound();
```

Double insertions and double removes are ignored.

### Complexity
The Y-Fast Trie methods run in the following time complexities where M is the size of universe.

| Operation | Time complexity |
| --- | --- |
| insert | O(log(log(M))) |
| remove | O(log(log(M))) |
| predecessor | O(log(log(M))) |
| successor | O(log(log(M))) |
| min | O(log(log(M))) |
| max | O(log(log(M))) |
| contains | O(1) |
| lower_bound | O(1) |
| upper_bound | O(1) |

### Example

Sample Program:
```c++
#include "../src/y-fast-trie/y-fast-trie.h"
#include <iostream>
#include <cstdint>

int main() {
    YFastTrie<uint32_t> trie;

    trie.insert(5);
    if (trie.contains(5))
        std::cout << "The trie contains the number 5." << std::endl;

    trie.insert(5);
    if (trie.contains(5))
        std::cout << "Double insertions are effectively no-ops." << std::endl;
    
    trie.remove(5);
    if (!trie.contains(5))
        std::cout << "The trie does not contain the number 5." << std::endl;
    
    trie.remove(5);
    if (!trie.contains(5))
        std::cout << "Double removes are effectively no-ops." << std::endl;
    
    trie.insert(7);
    trie.insert(11);
    trie.insert(16);

    std::cout << "The successor of 11 is "  << trie.successor(11).value() << "." << std::endl;
    std::cout << "The predecessor of 11 is "  << trie.predecessor(11).value() << "." << std::endl;

    std::cout << "The successor of 10 is "  << trie.successor(10).value() << "." << std::endl;
    std::cout << "The predecessor of 10 is "  << trie.predecessor(10).value() << "." << std::endl;

    std::cout << "Does the successor of 16 exist? "  << trie.successor(16).has_value() << "." << std::endl;
    std::cout << "Does the predecessor of 7 exist? "  << trie.predecessor(7).has_value() << "." << std::endl;
}
```

Sample Output:
```
The trie contains the number 5.
Double insertions are effectively no-ops.
The trie does not contain the number 5.
Double removes are effectively no-ops.
The successor of 11 is 16.
The predecessor of 11 is 7.
The successor of 10 is 11.
The predecessor of 10 is 7.
Does the successor of 16 exist? 0.
Does the predecessor of 7 exist? 0.
```
