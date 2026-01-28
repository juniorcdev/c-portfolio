#ifndef VECTOR_INT_H
#define VECTOR_INT_H

#include <stdbool.h>
#include <stddef.h>

/*
 * IntVector - A dynamic array for integers
 * 
 * Features:
 * - Amortized O(1) push/pop operations
 * - Automatic memory management with growth/shrink
 * - Bounds checking on access
 * - Clean, consistent API
 */

typedef struct IntVector IntVector;

//Creation and Destruction
IntVector *int_vector_create(size_t initial_capacity);          // Create a new vector with given initial capacity (0 for default)
void int_vector_destroy(IntVector *vec);                        // Destroy a vector, freeing all memory
void int_vector_clear(IntVector *vec);                          // Clear all elements from vector (size=0, keeps capacity)


// Basic Operations
void int_vector_push(IntVector *vec, int value);                // Append value to end of vector (amortized O(1))
int int_vector_pop(IntVector *vec);                             // Remove and return last element (O(1))
void int_vector_insert(IntVector *vec, size_t index, int value);// Insert value at specified index (shifts elements right)
void int_vector_remove(IntVector *vec, size_t index);           // Remove element at specified index (shifts elements left)

// Accessors
int int_vector_get(const IntVector *vec, size_t index);         // Get element at index (bounds checked)
void int_vector_set(IntVector *vec, size_t index, int value);   // Set element at index to value (bounds checked)
size_t int_vector_size(const IntVector *vec);                   // Get current number of elements
size_t int_vector_capacity(const IntVector *vec);               // Get current allocated capacity
bool int_vector_is_empty(const IntVector *vec);                 // Check if vector is empty
bool int_vector_is_full(const IntVector *vec);                  // Check if vector is full


// Memory Management
void int_vector_reserve(IntVector *vec, size_t new_capacity);   // Ensure vector has at least given capacity
void int_vector_shrink_to_fit(IntVector *vec);                  // Reduce capacity to match current size

// Search Operations
int int_vector_find(const IntVector *vec, int value);           // Find first occurrence of value, return index or -1 if not found
bool int_vector_contains(const IntVector *vec, int value);      // Check if vector contains value

// Utility Operations
IntVector *int_vector_copy(const IntVector *vec);               // Create a deep copy of vector
void int_vector_extend(IntVector *dest, const IntVector *src);  // Append all elements from another vector
bool int_vector_equals(const IntVector *a, const IntVector *b); // Compare two vectors for equality

#endif /* VECTOR_INT_H */