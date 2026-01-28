#include "../include/vector_int.h"
#include <stdio.h>

int main() {
    printf("=== Vector Example Usage ===\n\n");
    
    // Create a vector
    IntVector* vec = int_vector_create(5);
    printf("Created vector with capacity: %zu\n", int_vector_capacity(vec));
    
    // Push elements
    for (int i = 0; i < 10; i++) {
        int_vector_push(vec, i * i);
    }
    printf("After pushing 10 elements, size: %zu, capacity: %zu\n", 
           int_vector_size(vec), int_vector_capacity(vec));
    
    // Demonstrate various operations
    printf("\nFirst 5 elements: ");
    for (size_t i = 0; i < 5 && i < int_vector_size(vec); i++) {
        printf("%d ", int_vector_get(vec, i));
    }
    printf("\n");
    
    // Insert at position
    int_vector_insert(vec, 2, 999);
    printf("After inserting 999 at index 2, element[2] = %d\n", 
           int_vector_get(vec, 2));
    
    // Pop
    int popped = int_vector_pop(vec);
    printf("Popped value: %d, new size: %zu\n", popped, int_vector_size(vec));
    
    // Search
    int search_val = 16;
    int found_at = int_vector_find(vec, search_val);
    if (found_at != -1) {
        printf("Found %d at index %d\n", search_val, found_at);
    }
    
    // Copy
    IntVector* copy = int_vector_copy(vec);
    printf("Copied vector, sizes equal: %s\n", 
           int_vector_equals(vec, copy) ? "true" : "false");
    
    // Cleanup
    int_vector_destroy(vec);
    int_vector_destroy(copy);
    
    printf("\n=== Example completed successfully ===\n");
    return 0;
}