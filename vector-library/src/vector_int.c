#include "vector_int.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define GROWTH_FACTOR 2
#define DEFAULT_SIZE 8

struct IntVector {
    int *data;
    size_t size;
    size_t capacity;
};


// Helper functions
static bool int_vector_resize(IntVector *vec, size_t new_capacity) {
    assert(vec != NULL);
    assert(new_capacity >= vec->size);  // Can't shrink smaller than current size
    
    // Handle zero capacity case
    if (new_capacity == 0) {
        free(vec->data);
        vec->data = NULL;
        vec->capacity = 0;
        return true;
    }
    
    int* new_data = realloc(vec->data, new_capacity * sizeof(int));
    if (!new_data) {
        return false;
    }
    
    vec->data = new_data;
    vec->capacity = new_capacity;
    return true;
}


// Creation and Destruction
IntVector *int_vector_create(size_t initial_capacity) {
    IntVector *vec = malloc(sizeof(IntVector));
    if (!vec) return NULL;

    size_t cap = initial_capacity > 0 ? initial_capacity : DEFAULT_SIZE;
    vec->data = malloc(cap * sizeof(int));
    if (!vec->data) {
        free(vec);
        return NULL;
    }

    vec->size = 0;
    vec->capacity = cap;
    return vec;
}

void int_vector_destroy(IntVector *vec) {
    assert(vec != NULL);
    free(vec->data);
    free(vec);
}

void int_vector_clear(IntVector *vec) {
    assert(vec != NULL);
    vec->size = 0;
}


// Basic Operations
void int_vector_push(IntVector *vec, int value) {
    assert(vec != NULL);

     // Resize if needed (amortized O(1))
    if (vec->size == vec->capacity) {
        size_t new_cap = vec->capacity == 0 ? DEFAULT_SIZE : vec->capacity * GROWTH_FACTOR;
        bool success = int_vector_resize(vec, new_cap);
        assert(success); // Crash if resize fails
    }

    vec->data[vec->size++] = value;
}

int int_vector_pop(IntVector *vec) {
    assert(vec != NULL);
    assert(vec->size != 0);
    return vec->data[--vec->size];
}

void int_vector_insert(IntVector *vec, size_t index, int value) {
    assert(vec != NULL);
    assert(index <= vec->size);  // Can insert at end (size), but not beyond
    
    // Ensure capacity
    if (vec->size == vec->capacity) {
        size_t new_cap = vec->capacity == 0 ? DEFAULT_SIZE : vec->capacity * GROWTH_FACTOR;
        bool success = int_vector_resize(vec, new_cap);
        assert(success);
    }
    
    // Shift elements right to make space
    for (size_t i = vec->size; i > index; i--) {
        vec->data[i] = vec->data[i - 1];
    }
    
    // Insert new value
    vec->data[index] = value;
    vec->size++;
}

void int_vector_remove(IntVector *vec, size_t index) {
    assert(vec != NULL);
    assert(index < vec->size);  // Must be valid index
    
    // Shift elements left to fill gap
    for (size_t i = index; i < vec->size - 1; i++) {
        vec->data[i] = vec->data[i + 1];
    }
    
    vec->size--;
}


// Accessors
int int_vector_get(const IntVector* vec, size_t index) {
    assert(vec != NULL);
    assert(index < vec->size); // Bounds check
    return vec->data[index];
}

void int_vector_set(IntVector *vec, size_t index, int value) {
    assert(vec != NULL);
    assert(index < vec->size);  // Bounds check
    vec->data[index] = value;
}

size_t int_vector_size(const IntVector *vec) {
    assert(vec != NULL);
    return vec->size;
}

size_t int_vector_capacity(const IntVector *vec) {
    assert(vec != NULL);
    return vec->capacity;
}

bool int_vector_is_empty(const IntVector *vec) {
    assert(vec != NULL);
    return vec->size == 0;
}

bool int_vector_is_full(const IntVector *vec) {
    assert(vec != NULL);
    return vec->size == vec->capacity;
}


// Memory Management
void int_vector_reserve(IntVector *vec, size_t new_capacity) {
    assert(vec != NULL);
    if (new_capacity > vec->capacity) {
        bool success = int_vector_resize(vec, new_capacity);
        assert(success);
    }
}

void int_vector_shrink_to_fit(IntVector *vec) {
    assert(vec != NULL);
    if (vec->size < vec->capacity) {
        bool success = int_vector_resize(vec, vec->size);
        assert(success);
    }
}


// Search Operations
int int_vector_find(const IntVector *vec, int value) {
    assert(vec != NULL);
    
    for (size_t i = 0; i < vec->size; i++) {
        if (vec->data[i] == value) {
            return (int)i;  // Found - return index
        }
    }
    
    return -1;  // Not found
}

bool int_vector_contains(const IntVector *vec, int value) {
    assert(vec != NULL);
    return int_vector_find(vec, value) != -1;
}


// Utility Operations
IntVector *int_vector_copy(const IntVector *vec) {
    assert(vec != NULL);
    
    IntVector *copy = int_vector_create(vec->capacity);
    if (!copy) return NULL;
    
    // Copy data
    for (size_t i = 0; i < vec->size; i++) {
        copy->data[i] = vec->data[i];
    }
    
    copy->size = vec->size;
    return copy;
}

void int_vector_extend(IntVector *dest, const IntVector *src) {
    assert(dest != NULL);
    assert(src != NULL);
    
    // Ensure destination has enough capacity
    size_t new_size = dest->size + src->size;
    if (new_size > dest->capacity) {
        size_t new_cap = dest->capacity;
        while (new_cap < new_size) {
            new_cap = new_cap == 0 ? DEFAULT_SIZE : new_cap * GROWTH_FACTOR;
        }
        bool success = int_vector_resize(dest, new_cap);
        assert(success);
    }
    
    // Copy elements
    for (size_t i = 0; i < src->size; i++) {
        dest->data[dest->size + i] = src->data[i];
    }
    
    dest->size = new_size;
}

bool int_vector_equals(const IntVector *a, const IntVector *b) {
    assert(a != NULL);
    assert(b != NULL);
    
    if (a->size != b->size) return false;
    
    for (size_t i = 0; i < a->size; i++) {
        if (a->data[i] != b->data[i]) return false;
    }
    
    return true;
}