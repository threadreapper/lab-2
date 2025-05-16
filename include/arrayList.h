#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "allocator.h"
#include <stddef.h> 

typedef struct {
    void** data;
    size_t size;
    size_t capacity;
    Allocator* allocator;
} ArrayList;

void arraylist_init(ArrayList* list, Allocator* allocator);
void arraylist_add(ArrayList* list, void* data, size_t index);
void* arraylist_get(ArrayList* list, size_t index);
void arraylist_del(ArrayList* list, size_t index);
void arraylist_free(ArrayList* list);

#endif 
