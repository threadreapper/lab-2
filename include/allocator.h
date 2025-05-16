#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h> 

typedef struct Allocator {
    void* (*alloc)(struct Allocator* allocator, size_t size);
    void (*free)(struct Allocator* allocator, void* ptr);
} Allocator;

typedef struct {
    Allocator interface;  
    void* memory_block;
    size_t total_size;
    size_t offset;
} LinearAllocator;

typedef struct {
    Allocator interface;  
    void* memory_block;
    size_t block_size;
    size_t num_blocks;
    void* free_list;
} PoolAllocator;

void linear_init(LinearAllocator* allocator, size_t size);
void pool_init(PoolAllocator* allocator, size_t block_size, size_t num_blocks);

#endif 