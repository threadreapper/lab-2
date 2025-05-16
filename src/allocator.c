#include "allocator.h"
#include <stdlib.h>
#include <string.h>

static void* linear_alloc_impl(Allocator* allocator, size_t size) {
    LinearAllocator* lin_alloc = (LinearAllocator*)allocator;
    if (lin_alloc->offset + size > lin_alloc->total_size) {
        return NULL;
    }
    
    void* ptr = (char*)lin_alloc->memory_block + lin_alloc->offset;
    lin_alloc->offset += size;
    return ptr;
}

static void linear_free_impl(Allocator* allocator, void* ptr) {
    (void)allocator;
    (void)ptr;
}

void linear_init(LinearAllocator* allocator, size_t size) {
    allocator->interface.alloc = linear_alloc_impl;
    allocator->interface.free = linear_free_impl;
    allocator->memory_block = malloc(size);
    allocator->total_size = size;
    allocator->offset = 0;
}

static void* pool_alloc_impl(Allocator* allocator, size_t size) {
    PoolAllocator* pool_alloc = (PoolAllocator*)allocator;
    (void)size;
    
    if (pool_alloc->free_list == NULL) {
        return NULL;
    }
    
    void* ptr = pool_alloc->free_list;
    pool_alloc->free_list = *(void**)ptr;
    return ptr;
}

static void pool_free_impl(Allocator* allocator, void* ptr) {
    if (ptr == NULL) return;
    
    PoolAllocator* pool_alloc = (PoolAllocator*)allocator;
    *(void**)ptr = pool_alloc->free_list;
    pool_alloc->free_list = ptr;
}

void pool_init(PoolAllocator* allocator, size_t block_size, size_t num_blocks) {
    allocator->interface.alloc = pool_alloc_impl;
    allocator->interface.free = pool_free_impl;
    allocator->block_size = block_size;
    allocator->num_blocks = num_blocks;
    allocator->memory_block = malloc(block_size * num_blocks);
    allocator->free_list = allocator->memory_block;
    
    void* current = allocator->memory_block;
    for (size_t i = 0; i < num_blocks - 1; ++i) {
        void** next = (void**)((char*)current + block_size);
        *next = (char*)current + block_size;
        current = (char*)current + block_size;
    }
    *(void**)current = NULL;
}
