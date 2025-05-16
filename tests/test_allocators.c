#include "allocator.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_linear_allocator() {
    printf("Testing LinearAllocator...\n");
    
    LinearAllocator alloc;
    linear_init(&alloc, 1024);
    
    void* ptr1 = linear_alloc(&alloc, 100);
    assert(ptr1 != NULL && "Allocation failed");
    
    void* ptr2 = linear_alloc(&alloc, 200);
    assert(ptr2 != NULL && "Allocation failed");
    assert((char*)ptr2 == (char*)ptr1 + 100 && "Not sequential allocation");
    
    void* ptr3 = linear_alloc(&alloc, 1024);
    assert(ptr3 == NULL && "Should return NULL when out of memory");
    
    linear_reset(&alloc);
    void* ptr4 = linear_alloc(&alloc, 500);
    assert(ptr4 != NULL && "Allocation after reset failed");
    
    linear_free(&alloc);
    printf("LinearAllocator tests passed!\n\n");
}

void test_pool_allocator() {
    printf("Testing PoolAllocator...\n");
    
    PoolAllocator alloc;
    pool_init(&alloc, 32, 10);
    
    void* blocks[10];
    for (int i = 0; i < 10; i++) {
        blocks[i] = pool_alloc(&alloc);
        assert(blocks[i] != NULL && "Allocation failed");
    }
    
    void* overflow = pool_alloc(&alloc);
    assert(overflow == NULL && "Should return NULL when pool is full");
    
    pool_free(&alloc, blocks[3]);
    void* reused = pool_alloc(&alloc);
    assert(reused == blocks[3] && "Should reuse freed block");
    
    pool_free_all(&alloc);
    printf("PoolAllocator tests passed!\n\n");
}

int main() {
    test_linear_allocator();
    test_pool_allocator();
    return 0;
}
