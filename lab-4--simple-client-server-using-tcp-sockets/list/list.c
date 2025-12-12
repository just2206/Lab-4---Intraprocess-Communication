#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

// Helper function to create a new node
node_t *node_alloc(int value) {
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (node == NULL) {
        perror("Failed to allocate node");
        exit(EXIT_FAILURE);
    }
    node->value = value;
    node->next = NULL;
    return node;
}

// Allocates and initializes a new list structure
list_t *list_alloc() {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    if (list == NULL) {
        perror("Failed to allocate list");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->length = 0;
    return list;
}

// Frees all memory associated with the list and its nodes
void list_free(list_t *list) {
    if (list == NULL) return;
    node_t *current = list->head;
    node_t *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

// Returns the number of elements in the list
int list_length(list_t *list) {
    if (list == NULL) return 0;
    return list->length;
}

// Adds a new node to the front of the list
void list_add_to_front(list_t *list, int value) {
    if (list == NULL) return;
    node_t *new_node = node_alloc(value);
    new_node->next = list->head;
    list->head = new_node;
    list->length++;
}

// Adds a new node to the back of the list
void list_add_to_back(list_t *list, int value) {
    if (list == NULL) return;
    node_t *new_node = node_alloc(value);
    
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        node_t *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->length++;
}

// Adds a new node at a specific index
void list_add_at_index(list_t *list, int value, int index) {
    if (list == NULL || index < 0) return;

    if (index == 0) {
        list_add_to_front(list, value);
        return;
    }

    if (index > list->length) {
        // Option to add to back if index is out of bounds
        list_add_to_back(list, value); 
        return;
    }

    node_t *new_node = node_alloc(value);
    node_t *current = list->head;
    int i = 0;

    // Traverse to the node *before* the target index
    while (i < index - 1 && current != NULL) {
        current = current->next;
        i++;
    }

    if (current != NULL) {
        new_node->next = current->next;
        current->next = new_node;
        list->length++;
    }
}

// Removes and returns the value from the front of the list
int list_remove_from_front(list_t *list) {
    if (list == NULL || list->head == NULL) {
        return -1; // Sentinel value for error/empty
    }
    
    node_t *temp = list->head;
    int value = temp->value;
    list->head = list->head->next;
    free(temp);
    list->length--;
    
    return value;
}

// Removes and returns the value from the back of the list
int list_remove_from_back(list_t *list) {
    if (list == NULL || list->head == NULL) {
        return -1; // Sentinel value
    }

    if (list->head->next == NULL) { // Only one element
        return list_remove_from_front(list);
    }
    
    node_t *current = list->head;
    node_t *prev = NULL;
    
    // Traverse to the last node
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }
    
    int value = current->value;
    prev->next = NULL;
    free(current);
    list->length--;
    
    return value;
}

// Removes and returns the value at a specific index
int list_remove_at_index(list_t *list, int index) {
    if (list == NULL || list->head == NULL || index < 0 || index >= list->length) {
        return -1; // Sentinel value for error/out of bounds
    }

    if (index == 0) {
        return list_remove_from_front(list);
    }
    
    node_t *current = list->head;
    node_t *prev = NULL;
    int i = 0;

    // Traverse to the node *before* the target index
    while (i < index && current != NULL) {
        prev = current;
        current = current->next;
        i++;
    }

    if (current == NULL) {
        return -1; // Should not happen if index check is correct
    }

    int value = current->value;
    prev->next = current->next;
    free(current);
    list->length--;
    
    return value;
}

// Returns the element value at a specific index
int list_get_elem_at(list_t *list, int index) {
    if (list == NULL || list->head == NULL || index < 0 || index >= list->length) {
        return -1; // Sentinel value for error/out of bounds
    }
    
    node_t *current = list->head;
    int i = 0;
    while (i < index && current != NULL) {
        current = current->next;
        i++;
    }

    if (current != NULL) {
        return current->value;
    }

    return -1; // Should not happen
}


// Returns a string representation of the list contents
// Format: "[v1, v2, v3]"
char *listToString(list_t *list) {
    // Estimate max size of string: 10 chars per integer + ", " + 2 for "[]"
    // Use a conservative buffer size. 1024 is the max buffer size in serv.c
    if (list == NULL || list->head == NULL) {
        return strdup("[]");
    }

    char *str = (char *)malloc(1024);
    if (str == NULL) {
        perror("Failed to allocate string buffer");
        return strdup("[]"); 
    }
    
    // Start the string
    strcpy(str, "[");
    node_t *current = list->head;
    char temp_val[20]; 

    while (current != NULL) {
        sprintf(temp_val, "%d", current->value);
        strcat(str, temp_val);

        if (current->next != NULL) {
            strcat(str, ", ");
        }
        current = current->next;
    }

    strcat(str, "]");

    return str;
}