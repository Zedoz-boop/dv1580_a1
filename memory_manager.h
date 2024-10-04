#ifndef memory_manager_h
#define memory_manager_h

#include <stddef.h>  // For size_t
#include <stdbool.h> // For bool

// Define the mem_struct and function prototypes for the memory manager

typedef struct mem_struct {
    struct mem_struct *next;
    bool available;
    size_t size;
    void *memaddress;
} mem_struct;

// Function declarations 
void mem_init(size_t size);
void *mem_alloc(size_t size);
void mem_free(void* block);
void* mem_resize(void* block, size_t size);
void mem_deinit();
void coalesce_free_blocks();

#endif // MEMORY_MANAGER_H