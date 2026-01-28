#include "../include/vector_int.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Helper to track test results
static int tests_passed = 0;

#define RUN_TEST(test_func) do { \
    printf("Testing %-25s ... ", #test_func); \
    test_func(); \
    printf("PASS\n"); \
    tests_passed++; \
} while(0)

// ==================== Test Cases ====================

static void test_create_destroy() {
    // Test with explicit capacity
    IntVector* vec = int_vector_create(10);
    assert(vec != NULL);
    assert(int_vector_size(vec) == 0);
    assert(int_vector_capacity(vec) == 10);
    assert(int_vector_is_empty(vec));
    int_vector_destroy(vec);
    
    // Test with default capacity
    vec = int_vector_create(0);
    assert(vec != NULL);
    assert(int_vector_capacity(vec) == 8);  // DEFAULT_SIZE
    int_vector_destroy(vec);
}

static void test_push_pop() {
    IntVector* vec = int_vector_create(2);
    
    // Basic push
    int_vector_push(vec, 10);
    assert(int_vector_size(vec) == 1);
    assert(int_vector_get(vec, 0) == 10);
    
    // Multiple pushes with resize
    int_vector_push(vec, 20);
    int_vector_push(vec, 30);
    assert(int_vector_size(vec) == 3);
    assert(int_vector_capacity(vec) >= 4);  // Doubled
    
    // Verify values
    assert(int_vector_get(vec, 0) == 10);
    assert(int_vector_get(vec, 1) == 20);
    assert(int_vector_get(vec, 2) == 30);
    
    // Pop
    assert(int_vector_pop(vec) == 30);
    assert(int_vector_size(vec) == 2);
    assert(int_vector_pop(vec) == 20);
    assert(int_vector_pop(vec) == 10);
    assert(int_vector_is_empty(vec));
    
    int_vector_destroy(vec);
}

static void test_accessors() {
    IntVector* vec = int_vector_create(5);
    
    // Fill with values
    for (int i = 0; i < 5; i++) {
        int_vector_push(vec, i * 10);
    }
    
    // Test get/set
    assert(int_vector_get(vec, 2) == 20);
    int_vector_set(vec, 2, 99);
    assert(int_vector_get(vec, 2) == 99);
    
    // Test size/capacity
    assert(int_vector_size(vec) == 5);
    assert(int_vector_capacity(vec) >= 5);
    
    // Test is_empty/is_full
    assert(!int_vector_is_empty(vec));
    assert(int_vector_is_full(vec));
    
    // Clear and test
    int_vector_clear(vec);
    assert(int_vector_is_empty(vec));
    assert(int_vector_size(vec) == 0);
    
    int_vector_destroy(vec);
}

static void test_insert_remove() {
    IntVector* vec = int_vector_create(5);
    
    // Setup: [10, 20, 30]
    int_vector_push(vec, 10);
    int_vector_push(vec, 20);
    int_vector_push(vec, 30);
    
    // Insert at middle
    int_vector_insert(vec, 1, 15);
    assert(int_vector_size(vec) == 4);
    assert(int_vector_get(vec, 1) == 15);
    
    // Insert at beginning
    int_vector_insert(vec, 0, 5);
    assert(int_vector_get(vec, 0) == 5);
    
    // Insert at end
    int_vector_insert(vec, int_vector_size(vec), 40);
    assert(int_vector_get(vec, 5) == 40);
    
    // Remove from middle
    int_vector_remove(vec, 2);
    assert(int_vector_get(vec, 2) == 20);
    
    // Remove first
    int_vector_remove(vec, 0);
    assert(int_vector_get(vec, 0) == 10);
    
    // Remove last
    int_vector_remove(vec, int_vector_size(vec) - 1);
    assert(int_vector_size(vec) == 3);
    
    int_vector_destroy(vec);
}

static void test_memory_management() {
    IntVector* vec = int_vector_create(10);
    
    // Push some elements
    for (int i = 0; i < 5; i++) {
        int_vector_push(vec, i);
    }
    
    // Test reserve
    int_vector_reserve(vec, 20);
    assert(int_vector_capacity(vec) == 20);
    assert(int_vector_size(vec) == 5);
    
    // Verify data intact
    for (int i = 0; i < 5; i++) {
        assert(int_vector_get(vec, i) == i);
    }
    
    // Test shrink_to_fit
    int_vector_shrink_to_fit(vec);
    assert(int_vector_capacity(vec) == 5);
    
    int_vector_destroy(vec);
}

static void test_search() {
    IntVector* vec = int_vector_create(10);
    
    int_vector_push(vec, 10);
    int_vector_push(vec, 20);
    int_vector_push(vec, 30);
    int_vector_push(vec, 20);  // Duplicate
    
    // Test find
    assert(int_vector_find(vec, 10) == 0);
    assert(int_vector_find(vec, 20) == 1);  // First occurrence
    assert(int_vector_find(vec, 30) == 2);
    assert(int_vector_find(vec, 99) == -1);  // Not found
    
    // Test contains
    assert(int_vector_contains(vec, 10));
    assert(int_vector_contains(vec, 20));
    assert(!int_vector_contains(vec, 99));
    
    int_vector_destroy(vec);
}

static void test_utilities() {
    // Create source vector
    IntVector* src = int_vector_create(5);
    for (int i = 0; i < 5; i++) {
        int_vector_push(src, i * 10);
    }
    
    // Test copy
    IntVector* copy = int_vector_copy(src);
    assert(copy != NULL);
    assert(int_vector_equals(src, copy));
    
    // Modify copy, verify independence
    int_vector_set(copy, 0, 99);
    assert(!int_vector_equals(src, copy));
    
    // Test extend
    IntVector* dest = int_vector_create(2);
    int_vector_push(dest, 100);
    int_vector_extend(dest, src);
    assert(int_vector_size(dest) == 6);
    assert(int_vector_get(dest, 0) == 100);
    assert(int_vector_get(dest, 5) == 40);
    
    // Test equals with different vectors
    IntVector* a = int_vector_create(3);
    IntVector* b = int_vector_create(3);
    for (int i = 0; i < 3; i++) {
        int_vector_push(a, i);
        int_vector_push(b, i);
    }
    assert(int_vector_equals(a, b));
    
    int_vector_push(a, 99);
    assert(!int_vector_equals(a, b));
    
    // Cleanup
    int_vector_destroy(src);
    int_vector_destroy(copy);
    int_vector_destroy(dest);
    int_vector_destroy(a);
    int_vector_destroy(b);
}

static void test_edge_cases() {
    // Empty vector
    IntVector* empty = int_vector_create(5);
    assert(int_vector_is_empty(empty));
    assert(int_vector_find(empty, 42) == -1);
    
    // Copy empty
    IntVector* empty_copy = int_vector_copy(empty);
    assert(int_vector_equals(empty, empty_copy));
    
    // Extend with empty
    IntVector* vec = int_vector_create(3);
    int_vector_push(vec, 1);
    int_vector_extend(vec, empty);
    assert(int_vector_size(vec) == 1);
    
    int_vector_destroy(empty);
    int_vector_destroy(empty_copy);
    int_vector_destroy(vec);
}

int main() {
    printf("========================================\n");
    printf("       IntVector Test Suite\n");
    printf("========================================\n\n");
    
    // Run all tests
    RUN_TEST(test_create_destroy);
    RUN_TEST(test_push_pop);
    RUN_TEST(test_accessors);
    RUN_TEST(test_insert_remove);
    RUN_TEST(test_memory_management);
    RUN_TEST(test_search);
    RUN_TEST(test_utilities);
    RUN_TEST(test_edge_cases);
    
    printf("\n========================================\n");
    printf("All %d tests passed!\n", tests_passed);
    printf("========================================\n");
    
    return 0;
}