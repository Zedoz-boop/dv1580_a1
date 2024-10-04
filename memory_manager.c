#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct mem_struct{
    struct mem_struct *next;
    bool available;
    size_t size;
    void *memaddress;
} mem_struct;

mem_struct *head = NULL;

void mem_init(size_t size) {

    head = (mem_struct *)malloc(sizeof(mem_struct) * size);
    
    if (head == NULL) {
        //debug
        // printf("Failed to initialize memory manager.\n");
        return;
    }
    
    // Allocate the actual memory space that will be managed
    head->memaddress = (char*)head + sizeof(mem_struct);
    
    if (head->memaddress == NULL) {
        //debug
        // printf("Failed to allocate managed memory.\n");
        free(head);
        head = NULL;
        return;
    }
    
    head->next = NULL;
    head->available = true;
    head->size = size;
    //debug
    // printf("Memory initialized with size %zu bytes.\n", size);
}

void *mem_alloc(size_t size) {
    if (size == 0) {
        //debug
        // printf("Warning: Cannot allocate 0 bytes.\n");
        return (char*)head + sizeof(mem_struct);
    }

    mem_struct *current = head;
    
    // Traverse the linked list to find a suitable block
    while (current != NULL) {
        if (current->available && current->size >= size) {
            // Check if the block is large enough to be split
            if (current->size >= size + sizeof(mem_struct)) {
                // Create a new block from the remaining memory
                mem_struct *new_block = (mem_struct *)((char *)current->memaddress + size + sizeof(mem_struct));
                new_block->next = current->next;
                new_block->available = true;
                new_block->size = current->size - size; // Remaining memory

                // Set the memory address of the new block
                new_block->memaddress = (char *)new_block + sizeof(mem_struct);

                // Update the current block's metadata
                current->next = new_block;
                current->size = size;  
            }

            current->available = false; // Mark the current block as unavailable
            return current->memaddress; // Return the address of allocated memory
        }
        current = current->next; // Move to the next block in the list
    }

    return NULL;  // No suitable block found
}

void coalesce_free_blocks() {
    mem_struct *current = head;

    while (current != NULL && current->next != NULL) {
        // Check if current block and next block are both available
        if (current->available && current->next->available) {
            // Merge current block with the next block
            current->size += current->next->size;
            
            // Skip over the next block by adjusting the 'next' pointer
            mem_struct *next_block = current->next;
            current->next = next_block->next;
        } else {
            // Move to the next block in the list
            current = current->next;
        }
    }
}

void mem_free(void* block) {
    if (block == NULL) {
        printf("Warning: Trying to free a NULL pointer.\n");
        return;
    }

    mem_struct *current = head;

    // Traverse the linked list 
    while (current != NULL) {
        // Check if the requested block is the current block
        if (current->memaddress == block) {
            if (!current->available) {
                // Free the block
                current->available = true;
                coalesce_free_blocks();
                return;
            } else {
                //debug
                // printf("Error: Block at address %p is already free.\n", block);
                return;
            }
        }
        // Go to the next block
        current = current->next;
    }
    //debug
    // printf("Error: Block at address %p not found.\n", block);
}



void* mem_resize(void* block, size_t size) {
    // If block dosen't exist create it
    if (block == NULL) {
        return mem_alloc(size);
    }
    mem_struct * current = head;

    // Traverse the linked list 
    while (current != NULL){

        if (current->size >= size) {
                return block;  // No need to resize, the block is already large enough
        }
        if (current->memaddress == block) {
            
             // Try to resize in place by checking if the next block is free and large enough
            if (current->next != NULL && current->next->available &&
                (current->size + current->next->size ) >= size) {
                
                // Merge with the next block to increase size
                current->size += current->next->size ;
                mem_struct *next_block = current->next;
                current->next = next_block->next;
                
                // No need to move the block, just return the original address
                return current->memaddress;
            }

            // Otherwise, allocate a new block with the requested size
            void *new_block = mem_alloc(size);
            if (new_block == NULL) {
                //debug
                // printf("Error: Unable to allocate memory for resizing.\n");
                return NULL;
            }

            // Copy the old data to the new block (only up to the size of the old block)
            memcpy(new_block, block, current->size);

            // Free the old block
            mem_free(block);

            // Return the new block address
            return new_block;
        }
        // Go to next block
        current = current->next;
    }

    // If we reach here, the block was not found
    //debug
    // printf("Error: Block not found for resizing.\n");
    return NULL;
}

void mem_deinit() {
    // Check if linked list exist
    if (head == NULL) {
        //debug
        // printf("Memory manager is already deinitialized or not initialized.\n");
        return;
    }

    free(head);

    head = NULL;
    //debug
    // printf("Memory pool deinitialized and freed.\n");
}

