#include "arrayList.h"
#include <string.h>

void arraylist_init(ArrayList* list, Allocator* allocator) {
    list->size = 0;
    list->capacity = 16;
    list->allocator = allocator;
    list->data = allocator->alloc(allocator, sizeof(void*) * list->capacity);
}

void arraylist_add(ArrayList* list, void* data, size_t index) {
    if (index > list->size) return;
    
    if (list->size == list->capacity) {
        size_t new_capacity = list->capacity * 2;
        void** new_data = list->allocator->alloc(list->allocator, sizeof(void*) * new_capacity);
        
        memcpy(new_data, list->data, sizeof(void*) * list->size);
        
        list->allocator->free(list->allocator, list->data);
        
        list->data = new_data;
        list->capacity = new_capacity;
    }
    
    if (index < list->size) {
        memmove(&list->data[index+1], &list->data[index], 
                sizeof(void*) * (list->size - index));
    }
    
    list->data[index] = data;
    list->size++;
}

void* arraylist_get(ArrayList* list, size_t index) {
    if (index >= list->size) return NULL;
    return list->data[index];
}

void arraylist_del(ArrayList* list, size_t index) {
    if (index >= list->size) return;
    
    if (index < list->size - 1) {
        // Сдвигаем элементы влево
        memmove(&list->data[index], &list->data[index+1], 
                sizeof(void*) * (list->size - index - 1));
    }
    
    list->size--;
}

void arraylist_free(ArrayList* list) {
    if (list->data) {
        list->allocator->free(list->allocator, list->data);
        list->data = NULL;
    }
    list->size = 0;
    list->capacity = 0;
}
