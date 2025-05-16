#include "hashtable.h"
#include "allocator.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_hashtable_basic() {
    printf("Testing HashTable basic operations...\n");
    
    PoolAllocator alloc;
    pool_init(&alloc, sizeof(HashNode), 100);
    
    HashTable table;
    hashtable_init(&table, 16, (Allocator*)&alloc);
    
    int value1 = 42;
    float value2 = 3.14f;
    char* value3 = "test string";
    
    hashtable_insert(&table, "key1", &value1);
    hashtable_insert(&table, "key2", &value2);
    hashtable_insert(&table, "key3", value3);
    
    assert(*(int*)hashtable_get(&table, "key1") == 42 && "Wrong value");
    assert(*(float*)hashtable_get(&table, "key2") == 3.14f && "Wrong value");
    assert(strcmp((char*)hashtable_get(&table, "key3"), "test string") == 0 && "Wrong value");
    
    int new_value = 100;
    hashtable_insert(&table, "key1", &new_value);
    assert(*(int*)hashtable_get(&table, "key1") == 100 && "Update failed");
    
    hashtable_remove(&table, "key2");
    assert(hashtable_get(&table, "key2") == NULL && "Remove failed");
    
    hashtable_free(&table);
    pool_free_all(&alloc);
    printf("HashTable basic tests passed!\n\n");
}

void test_hashtable_collisions() {
    printf("Testing HashTable collisions...\n");
    
    LinearAllocator alloc;
    linear_init(&alloc, 8192);
    
    HashTable table;
    hashtable_init(&table, 2, (Allocator*)&alloc);
    
    int values[10] = {0};
    for (int i = 0; i < 10; i++) {
        values[i] = i;
        char key[16];
        snprintf(key, sizeof(key), "key%d", i);
        hashtable_insert(&table, key, &values[i]);
    }
    
    for (int i = 0; i < 10; i++) {
        char key[16];
        snprintf(key, sizeof(key), "key%d", i);
        int* val = hashtable_get(&table, key);
        assert(val != NULL && *val == i && "Wrong value with collisions");
    }
    
    hashtable_free(&table);
    linear_free(&alloc);
    printf("HashTable collision tests passed!\n\n");
}

int main() {
    test_hashtable_basic();
    test_hashtable_collisions();
    return 0;
}