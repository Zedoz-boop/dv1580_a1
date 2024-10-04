#ifndef linked_list_h
#define linked_list_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef unsigned int uint16_t;


typedef struct Node {
    uint16_t data; // Stores the data as an unsigned 16-bit integer
    struct Node* next; // A pointer to the next node in the List
} Node;

void list_init(Node** list_head, size_t size);

void list_insert(Node** list_head, uint16_t data);

void list_insert_after(Node* prev_node, uint16_t data);

void list_insert_before(Node** list_head, Node* next_node, uint16_t data);

void list_delete(Node** list_head, uint16_t data);

Node* list_search(Node** list_head, uint16_t data);

void list_display(Node** list_head);

void list_display_range(Node** list_head, Node* start_node, Node* end_node);

int list_count_nodes(Node** list_head);

void list_cleanup(Node** list_head);

#endif