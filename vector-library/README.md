# IntVector - A Dynamic Array Library in C

A professional, feature-complete dynamic array (vector) implementation for integers, written in pure C. Built as a portfolio project to demonstrate data structure and systems programming skills for junior C developer roles.

## 🚀 Features

### Core Functionality

* **Dynamic resizing:** Automatically grows and shrinks as needed
* **Amortized O(1) operations:** Push/pop with doubling growth strategy
* **Type-safe:** Specifically for `int` types (cleaner than `void*` generics)
* **Memory efficient:** Proper allocation with `realloc`, no memory leaks

### Complete API

* **Creation/Destruction:** `create`, `destroy`, `clear`
* **Basic Operations:** `push`, `pop`, `insert`, `remove`
* **Accessors:** `get`, `set`, `size`, `capacity`, `is_empty`, `is_full`
* **Memory Management:** `reserve`, `shrink_to_fit`
* **Search Operations:** `find`, `contains`
* **Utility Functions:** `copy`, `extend`, `equals`

### Technical Highlights

* **Opaque pointer pattern:** Encapsulation via incomplete type (struct defined in .c file)
* **Bounds checking:** Assert-based preconditions during development
* **Professional build **system: Makefile with testing, examples, and installation
* **Comprehensive testing:** 50+ assertions across 8 test categories
* **Memory safe:** Valgrind-clean, proper error handling

## 📦 Installation & Building

### Prerequisites
* GCC or any C99-compatible compiler
* Make (for build system)
* Valgrind (optional, for memory leak checking)

### Quick Start
```bash
# Clone and build
git clone <repository-url>
cd vector_int

# Build everything (library, tests, examples)
make

# Run tests
make test

# Run example program
make example
```

### Build Targets
```bash
make all           # Build everything
make test          # Build and run tests
make example       # Build and run example
make valgrind-test # Run tests with memory checking
make clean         # Remove build artifacts
make install       # Install system-wide (requires sudo)
make help          # Show all available targets
```

## 🛠️ Usage

### Basic Example

```c
#include "vector_int.h"
#include <stdio.h>

int main() {
    // Create a vector with initial capacity of 5
    IntVector* vec = int_vector_create(5);
    
    // Push elements (automatically grows when needed)
    for (int i = 0; i < 10; i++) {
        int_vector_push(vec, i * 10);
    }
    
    // Access elements
    printf("Size: %zu, Capacity: %zu\n", 
           int_vector_size(vec), 
           int_vector_capacity(vec));
    
    // Iterate through vector
    for (size_t i = 0; i < int_vector_size(vec); i++) {
        printf("vec[%zu] = %d\n", i, int_vector_get(vec, i));
    }
    
    // Insert at position
    int_vector_insert(vec, 2, 999);
    
    // Remove element
    int_vector_remove(vec, 5);
    
    // Pop last element
    int last = int_vector_pop(vec);
    printf("Popped: %d\n", last);
    
    // Clean up
    int_vector_destroy(vec);
    return 0;
}
```

### Advanced Example
```c
// Creating a vector copy
IntVector* original = int_vector_create(10);
for (int i = 0; i < 5; i++) {
    int_vector_push(original, i * 100);
}

IntVector* copy = int_vector_copy(original);
printf("Vectors equal: %s\n", 
       int_vector_equals(original, copy) ? "true" : "false");

// Extending vectors
IntVector* combined = int_vector_create(3);
int_vector_extend(combined, original);
int_vector_extend(combined, copy);

// Memory management
int_vector_reserve(combined, 50);      // Pre-allocate capacity
int_vector_shrink_to_fit(combined);    // Reduce to exact size

// Searching
int index = int_vector_find(combined, 300);
if (index != -1) {
    printf("Found 300 at index %d\n", index);
}
```

## 🧪 Testing

The project includes a comprehensive test suite:

```bash
# Run all tests
make test

# Output:
========================================
       IntVector Test Suite
========================================

Testing test_create_destroy        ... PASS
Testing test_push_pop              ... PASS
Testing test_accessors             ... PASS
Testing test_insert_remove         ... PASS
Testing test_memory_management     ... PASS
Testing test_search                ... PASS
Testing test_utilities             ... PASS
Testing test_edge_cases            ... PASS

========================================
All 8 tests passed!
========================================
```

### Test Coverage

* ✅ Creation and destruction
* ✅ Push/pop with resizing
* ✅ Get/set and bounds checking
* ✅ Insert/remove with element shifting
* ✅ Memory management (reserve, shrink)
* ✅ Search operations (find, contains)
* ✅ Utility functions (copy, extend, equals)
* ✅ Edge cases (empty vectors, boundary conditions)

## 📊 Performance Characteristics


|Operation|Time Complexity|Notes|
|:--------|:--------------|:----|
|push|O(1)| amortized	Doubles capacity when full|
|pop|O(1)|No reallocation on shrink|
|get/set|O(1)|Direct array access|
|insert/remove|O(n)|Requires element shifting|
|find/contains|O(n)|Linear search|
|reserve|O(n)|May copy all elements|

**Memory overhead:** ~24 bytes per vector + capacity × sizeof(int)

### Technical Depth Demonstrated:

* **Data Structures:** Dynamic array implementation
* **Algorithms:** Amortized analysis, element shifting
* **Memory Management:** malloc/free/realloc, no leaks
* **C Programming:** Pointers, structs, encapsulation
* **Software Engineering:** API design, testing, documentation

## 🔧 Project Structure

```text
vector_int/
├── include/               # Public headers
│   └── vector_int.h      # API definition
├── src/                  # Implementation
│   └── vector_int.c      # Complete implementation
├── tests/                # Test suite
│   └── test_vector.c     # 50+ assertions
├── examples/             # Usage examples
│   └── example_usage.c   # Demonstration program
├── build/                # Build artifacts (created)
├── Makefile             # Professional build system
└── README.md            # This file
```