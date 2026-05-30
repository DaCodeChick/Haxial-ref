/**
 * @file list.c
 * @brief Intrusive doubly-linked list implementation
 */

#include "haxial/list.h"
#include <stddef.h>

/**
 * Internal structure definition
 */
struct TList {
    TListNode *head;
    TListNode *tail;
    size_t count;
};

void hx_list_init(TList *list) {
    if (list == NULL) {
        return;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

bool hx_list_is_empty(const TList *list) {
    if (list == NULL) {
        return true;
    }
    
    return list->head == NULL;
}

size_t hx_list_count(const TList *list) {
    if (list == NULL) {
        return 0;
    }
    
    return list->count;
}

void hx_list_prepend(TList *list, TListNode *node) {
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

void hx_list_append(TList *list, TListNode *node) {
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

void hx_list_remove(TList *list, TListNode *node) {
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

TListNode* hx_list_first(const TList *list) {
    if (list == NULL) {
        return NULL;
    }
    
    return list->head;
}

TListNode* hx_list_last(const TList *list) {
    if (list == NULL) {
        return NULL;
    }
    
    return list->tail;
}

TListNode* hx_list_next(const TListNode *node) {
    if (node == NULL) {
        return NULL;
    }
    
    return node->next;
}

TListNode* hx_list_prev(const TListNode *node) {
    if (node == NULL) {
        return NULL;
    }
    
    return node->prev;
}
