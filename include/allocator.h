#ifndef ALLOCATOR_H
#define ALLOCATOR_H
#include <stddef.h>

typedef struct {
    void* memory_block;
    size_t total_size;
    size_t offset;
} LinearAllocator;

typedef struct {
    void* memory_block;
    size_t block_size;
    size_t num_blocks;
    void* free_list;
} PoolAllocator;

void pool_init(PoolAllocator* allocator, size_t block_size, size_t num_blocks);
void* pool_alloc(PoolAllocator* allocator);
void pool_free(PoolAllocator* allocator, void* ptr);
void pool_free_all(PoolAllocator* allocator);

void linear_init(LinearAllocator* allocator, size_t size);
void* linear_alloc(LinearAllocator* allocator, size_t size);
void linear_reset(LinearAllocator* allocator);
void linear_free(LinearAllocator* allocator);

#endif // ALLOCATOR_H
