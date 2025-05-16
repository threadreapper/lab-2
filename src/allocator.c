#include "allocator.h"
#include <stdlib.h>
#include <string.h> 

void linear_init(LinearAllocator* allocator, size_t size) {
    allocator->memory_block = malloc(size);
    allocator->total_size = size;
    allocator->offset = 0;
}

void* linear_alloc(LinearAllocator* allocator, size_t size) {
    if (allocator->offset + size > allocator->total_size) {
        return NULL;
    }
    
    void* ptr = (char*)allocator->memory_block + allocator->offset;
    allocator->offset += size;
    return ptr;
}

void linear_reset(LinearAllocator* allocator) {
    allocator->offset = 0;
}

void linear_free(LinearAllocator* allocator) {
    free(allocator->memory_block);
    allocator->memory_block = NULL;
    allocator->total_size = 0;
    allocator->offset = 0;
}

// allocator.c
void pool_init(PoolAllocator* allocator, size_t block_size, size_t num_blocks) {
    allocator->block_size = block_size;
    allocator->num_blocks = num_blocks;
    allocator->memory_block = malloc(block_size * num_blocks);
    allocator->free_list = allocator->memory_block;
    
    // Initialize free list
    void* current = allocator->memory_block;
    for (size_t i = 0; i < num_blocks - 1; ++i) {
        void** next = (void**)((char*)current + block_size);
        *next = (char*)current + block_size;
        current = (char*)current + block_size;
    }
    *(void**)current = NULL;
}

void* pool_alloc(PoolAllocator* allocator) {
    if (allocator->free_list == NULL) {
        return NULL;
    }
    
    void* ptr = allocator->free_list;
    allocator->free_list = *(void**)ptr;
    return ptr;
}

void pool_free(PoolAllocator* allocator, void* ptr) {
    if (ptr == NULL) return;
    
    *(void**)ptr = allocator->free_list;
    allocator->free_list = ptr;
}

void pool_free_all(PoolAllocator* allocator) {
    free(allocator->memory_block);
    allocator->memory_block = NULL;
    allocator->free_list = NULL;
}
