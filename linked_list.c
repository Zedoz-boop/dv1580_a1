#include "memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef unsigned int uint16_t;

typedef struct Node {
    uint16_t data; // Stores the data as an unsigned 16-bit integer
    struct Node* next; // A pointer to the next node in the List
} Node;

void list_init(Node** list_head, size_t size) {
    // Initialize memory for the list using mem_init
    mem_init(size+sizeof(Node));

    *list_head = NULL;

    //debug
    // printf("Linked list initialized with memory size: %zu bytes.\n", size);
}

void list_insert(Node** list_head, uint16_t data) {
    // Allocate memory for the new node using mem_alloc
    Node* new_node = (Node*) mem_alloc(sizeof(Node));
    
    if (new_node == NULL) {
        //debug
        // printf("Failed to allocate memory for new node.\n");
        return;
    }

    // Set the data for the new node
    new_node->data = data;
    new_node->next = NULL; 

    // If the list is empty, make the new node the head
    if (*list_head == NULL) {
        *list_head = new_node;
        //debug
        // printf("Node with data %u inserted as the head.\n", data);
        return;
    }

    // Otherwise, find the last node
    Node* current = *list_head;
    while (current->next != NULL) {
        current = current->next;  // Traverse until we reach the last node
    }

    // Append the new node at the rear end
    current->next = new_node;
    //debug
    // printf("Node with data %u inserted at the rear end.\n", data);
}


void list_insert_after(Node* prev_node, uint16_t data) {
    
    if (prev_node == NULL) {
        //debug
        // printf("Previus node cannot be NULL.\n");
        return;
    }

    // Allocate memory for the new node
    Node* new_node = (Node*) mem_alloc(sizeof(Node));

    if (new_node == NULL) {
        //debug
        // printf("Failed to allocate memory for new node.\n");
        return;
    }

    new_node->data = data;

    // Make the new node's next point to the previous node's next
    new_node->next = prev_node->next;

    // Make the previous node point to the new node
    prev_node->next = new_node;
    //debug
    // printf("Node with data %u inserted after node with data %u.\n", data, prev_node->data);
}

void list_insert_before(Node** list_head, Node* next_node, uint16_t data) {
    if (next_node == NULL) {
        //debug
        // printf("Next node cannot be NULL.\n");
        return;
    }

    // Allocate memory for the new node
    Node* new_node = (Node*) mem_alloc(sizeof(Node));

    if (new_node == NULL) {
        //debug
        // printf("Failed to allocate memory for new node.\n");
        return;
    }

    Node* current = *list_head;

    // Check if next node is the first node
    if (next_node == *list_head) {
        // Set the new node to the head
        *list_head = new_node;
        new_node->data = data;
        new_node->next = current;
        //debug
        // printf("Node with data %u inserted as the head before node with data %u.\n", data, next_node->data);
    }
    else { 
        // Traverse the list to before the next node
        while (current->next != next_node) {
            current = current->next;  
        }
        // Set the new nodes data
        new_node->data = data;
        new_node->next = current->next;
        current->next = new_node;
    }

    //debug
    // printf("Node with data %u inserted before node with data %u.\n", data, next_node->data);
}

void list_delete(Node** list_head, uint16_t data) {
    if (list_head == NULL) {
        //debug
        // printf("Cannot delete from an empty list. \n");
        return;
    }

    Node* current = *list_head;
    Node* prev = NULL;

    // Traverse to find the node to delete
    while (current != NULL && current->data != data) {
        prev = current;
        current = current->next;
    }

    // If node is not found
    if (current == NULL) {
        //debug
        // printf("Node with data %u not found.\n", data);
        return;
    }

    // If node to be deleted is the head
    if (prev == NULL) {
        *list_head = current->next;  
    } else {
        prev->next = current->next;  
    }

    // Free the memory of the deleted node using mem_free
    mem_free(current);
    //debug
    // printf("Node with data %u deleted.\n", data);
}

Node* list_search(Node** list_head, uint16_t data) {
    if (*list_head == NULL) {
        //debug
        // printf("List is empty.\n");
        return NULL;
    }

    Node* current = *list_head;

    // Traverse the list until the end or the node is found 
    while (current != NULL) {
        if (current->data == data){
            // return node
            return current;
        }
        current = current->next;
    }
    // If node not found return NULL
    return NULL;
}

void list_display(Node** list_head) {
    Node * current = *list_head;

    // If the list is empty
    if (current == NULL) {
        printf("[]");
        return;
    }

    printf("[");
    
    // Traverse the list and print each element
    while (current != NULL) {
        printf("%u", current->data);
        current = current->next;
        
        // If we are at the last node dont print ", "
        if (current != NULL) {
            printf(", ");  // Print comma after each element except the last one
        }
    }

    printf("]");
}

void list_display_range(Node** list_head, Node* start_node, Node* end_node) {
    
    Node * current = *list_head;

    // If the list is empty
    if (current == NULL) {
        printf("[]");
        return;
    }

    printf("[");
    
    bool start = false;
    // Traverse the list and print each element
    while (current != NULL) {
        // If start_node is null set start node to the first node
        if (start_node == NULL) {
            start_node = current;
        }
        // start with start node and continue with start variabel till end or break
        if (start_node == current || start == true) {

            printf("%u", current->data);

            if (current->next != NULL && current != end_node) {
                printf(", ");  // Print comma after each element except the last one
            }  
            start = true;
        }
        // Break if we reached the end
        if (current == end_node) {
            break;
        }
      
        current = current->next;
    }

    printf("]");

}

int list_count_nodes(Node** list_head) {
    Node* current = *list_head;  
    int count = 0;
    // If the list is empty
    if (current == NULL) {
        //debug
        // printf("0\n");
        return 0;
    }
    // Traverse the list and count
    while(current != NULL) {
        count += 1;
        current = current->next;
    }
    //debug
    // printf("The list has %u nodes. \n", count);
    return count;
}

void list_cleanup(Node** list_head) {
    Node* current = *list_head;
    Node* next_node;

    // Traverse and free all nodes
    while (current != NULL) {
        next_node = current->next;  // Save the next node
        mem_free(current);          // Free the current node using the custom memory manager
        current = next_node;        // Move to the next node
    }

    // Set head to NULL after all nodes are freed
    *list_head = NULL;
    //debug
    // printf("All nodes have been cleaned up and memory has been freed.\n");

}