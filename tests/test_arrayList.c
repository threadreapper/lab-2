#include "arrayList.h"
#include "allocator.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_arraylist_basic() {
    printf("Testing ArrayList basic operations...\n");
    
    LinearAllocator alloc;
    linear_init(&alloc, 4096);
    
    ArrayList list;
    arraylist_init(&list, (Allocator*)&alloc);
    
    int values[5] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        arraylist_add(&list, &values[i], i);
    }
    
    for (int i = 0; i < 5; i++) {
        int* val = arraylist_get(&list, i);
        assert(val != NULL && *val == values[i] && "Wrong value");
    }
    
    assert(list.size == 5 && "Wrong size");
    
    arraylist_del(&list, 2);
    assert(list.size == 4 && "Wrong size after delete");
    assert(*(int*)arraylist_get(&list, 2) == 40 && "Wrong element after delete");
    
    arraylist_free(&list);
    linear_free(&alloc);
    printf("ArrayList basic tests passed!\n\n");
}

void test_arraylist_expansion() {
    printf("Testing ArrayList expansion...\n");
    
    LinearAllocator alloc;
    linear_init(&alloc, 8192);
    
    ArrayList list;
    arraylist_init(&list, (Allocator*)&alloc);
    
    for (int i = 0; i < 100; i++) {
        int* value = linear_alloc(&alloc, sizeof(int));
        *value = i;
        arraylist_add(&list, value, i);
    }
    
    assert(list.size == 100 && "Wrong size");
    assert(list.capacity >= 100 && "Capacity not increased");
    
    for (int i = 0; i < 100; i++) {
        assert(*(int*)arraylist_get(&list, i) == i && "Wrong value after expansion");
    }
    
    arraylist_free(&list);
    linear_free(&alloc);
    printf("ArrayList expansion tests passed!\n\n");
}

int main() {
    test_arraylist_basic();
    test_arraylist_expansion();
    return 0;
}
