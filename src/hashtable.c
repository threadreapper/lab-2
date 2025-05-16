#include "hashtable.h"
#include <stdlib.h>  
#include <string.h>  
#include <assert.h>  

static unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void hashtable_init(HashTable* table, size_t capacity, Allocator* allocator) {
    assert(table != NULL);
    assert(capacity > 0);
    assert(allocator != NULL);

    table->capacity = capacity;
    table->allocator = allocator;
    table->buckets = allocator->alloc(allocator, sizeof(HashNode*) * capacity);
    memset(table->buckets, 0, sizeof(HashNode*) * capacity);
}

void hashtable_insert(HashTable* table, const char* key, void* value) {
    assert(table != NULL);
    assert(key != NULL);

    unsigned long h = hash_function(key) % table->capacity;
    
    HashNode* node = table->buckets[h];
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;  // Обновляем значение
            return;
        }
        node = node->next;
    }

    HashNode* new_node = table->allocator->alloc(table->allocator, sizeof(HashNode));
    new_node->key = strdup(key);
    new_node->value = value;
    new_node->next = table->buckets[h];
    table->buckets[h] = new_node;
}

void* hashtable_get(HashTable* table, const char* key) {
    assert(table != NULL);
    assert(key != NULL);

    unsigned long h = hash_function(key) % table->capacity;
    
    HashNode* node = table->buckets[h];
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    
    return NULL;
}

void hashtable_remove(HashTable* table, const char* key) {
    assert(table != NULL);
    assert(key != NULL);

    unsigned long h = hash_function(key) % table->capacity;
    
    HashNode* prev = NULL;
    HashNode* current = table->buckets[h];
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                table->buckets[h] = current->next;
            } else {
                prev->next = current->next;
            }
            
            free(current->key);
            table->allocator->free(table->allocator, current);
            return;
        }
        
        prev = current;
        current = current->next;
    }
}

void hashtable_free(HashTable* table) {
    assert(table != NULL);

    for (size_t i = 0; i < table->capacity; ++i) {
        HashNode* node = table->buckets[i];
        while (node != NULL) {
            HashNode* next = node->next;
            free(node->key);
            table->allocator->free(table->allocator, node);
            node = next;
        }
    }
    
    table->allocator->free(table->allocator, table->buckets);
    table->buckets = NULL;
    table->capacity = 0;
}
