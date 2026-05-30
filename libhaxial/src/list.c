/**
 * @file list.c
 * @brief Intrusive doubly-linked list implementation
 */

#include "haxial/list.h"
#include <stddef.h>

void hx_list_init(HxList *list) {
    if (list == NULL) {
        return;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

bool hx_list_is_empty(const HxList *list) {
    if (list == NULL) {
        return true;
    }
    
    return list->head == NULL;
}

size_t hx_list_count(const HxList *list) {
    if (list == NULL) {
        return 0;
    }
    
    return list->count;
}

void hx_list_prepend(HxList *list, HxListNode *node) {
    if (list == NULL || node == NULL) {
        return;
    }
    
    node->prev = NULL;
    node->next = list->head;
    
    if (list->head != NULL) {
        list->head->prev = node;
    } else {
        // List was empty
        list->tail = node;
    }
    
    list->head = node;
    list->count++;
}

void hx_list_append(HxList *list, HxListNode *node) {
    if (list == NULL || node == NULL) {
        return;
    }
    
    node->next = NULL;
    node->prev = list->tail;
    
    if (list->tail != NULL) {
        list->tail->next = node;
    } else {
        // List was empty
        list->head = node;
    }
    
    list->tail = node;
    list->count++;
}

void hx_list_remove(HxList *list, HxListNode *node) {
    if (list == NULL || node == NULL) {
        return;
    }
    
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        // Removing head
        list->head = node->next;
    }
    
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        // Removing tail
        list->tail = node->prev;
    }
    
    node->next = NULL;
    node->prev = NULL;
    list->count--;
}

HxListNode* hx_list_first(const HxList *list) {
    if (list == NULL) {
        return NULL;
    }
    
    return list->head;
}

HxListNode* hx_list_last(const HxList *list) {
    if (list == NULL) {
        return NULL;
    }
    
    return list->tail;
}

HxListNode* hx_list_next(const HxListNode *node) {
    if (node == NULL) {
        return NULL;
    }
    
    return node->next;
}

HxListNode* hx_list_prev(const HxListNode *node) {
    if (node == NULL) {
        return NULL;
    }
    
    return node->prev;
}
