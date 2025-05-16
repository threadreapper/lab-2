#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "allocator.h"
#include <stddef.h>  
#include <string.h> 

typedef struct HashNode HashNode;

struct HashNode {
    char* key;
    void* value;
    HashNode* next;
};

typedef struct {
    size_t capacity;
    HashNode** buckets;
    Allocator* allocator;
} HashTable;

void hashtable_init(HashTable* table, size_t capacity, Allocator* allocator);
void hashtable_insert(HashTable* table, const char* key, void* value);
void* hashtable_get(HashTable* table, const char* key);
void hashtable_remove(HashTable* table, const char* key);
void hashtable_free(HashTable* table);

#endif 
