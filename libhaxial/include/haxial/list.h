/**
 * @file list.h
 * @brief Intrusive doubly-linked list implementation
 * 
 * This is an intrusive linked list where the list node structure is embedded
 * directly into the user's data structure, similar to Linux kernel lists.
 * 
 * Usage:
 *   struct MyData {
 *       int value;
 *       HxListNode node;  // Embed list node
 *   };
 * 
 *   HxList list;
 *   hx_list_init(&list);
 *   
 *   struct MyData *data = malloc(sizeof(struct MyData));
 *   data->value = 42;
 *   hx_list_append(&list, &data->node);
 */

#ifndef HAXIAL_LIST_H
#define HAXIAL_LIST_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Intrusive list node - embed this in your data structure
 */
typedef struct HxListNode {
    struct HxListNode *next;
    struct HxListNode *prev;
} HxListNode;

/**
 * List head structure
 */
typedef struct HxList {
    HxListNode *head;
    HxListNode *tail;
    size_t count;
} HxList;

/**
 * Initialize a list
 */
void hx_list_init(HxList *list);

/**
 * Check if list is empty
 */
bool hx_list_is_empty(const HxList *list);

/**
 * Get number of elements in list
 */
size_t hx_list_count(const HxList *list);

/**
 * Prepend node to front of list
 */
void hx_list_prepend(HxList *list, HxListNode *node);

/**
 * Append node to end of list
 */
void hx_list_append(HxList *list, HxListNode *node);

/**
 * Remove node from list
 */
void hx_list_remove(HxList *list, HxListNode *node);

/**
 * Get first node (or NULL if empty)
 */
HxListNode* hx_list_first(const HxList *list);

/**
 * Get last node (or NULL if empty)
 */
HxListNode* hx_list_last(const HxList *list);

/**
 * Get next node (or NULL if at end)
 */
HxListNode* hx_list_next(const HxListNode *node);

/**
 * Get previous node (or NULL if at start)
 */
HxListNode* hx_list_prev(const HxListNode *node);

/**
 * Get container structure from list node
 * 
 * @param ptr Pointer to the list node
 * @param type Type of the containing structure
 * @param member Name of the list node member in the structure
 */
#define HX_LIST_ENTRY(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

/**
 * Iterate over list
 * 
 * @param list Pointer to HxList
 * @param node HxListNode* variable for iteration
 */
#define HX_LIST_FOR_EACH(list, node) \
    for ((node) = (list)->head; (node) != NULL; (node) = (node)->next)

/**
 * Iterate over list safely (allows removal during iteration)
 * 
 * @param list Pointer to HxList
 * @param node HxListNode* variable for iteration
 * @param tmp HxListNode* variable for temporary storage
 */
#define HX_LIST_FOR_EACH_SAFE(list, node, tmp) \
    for ((node) = (list)->head, (tmp) = (node) ? (node)->next : NULL; \
         (node) != NULL; \
         (node) = (tmp), (tmp) = (node) ? (node)->next : NULL)

#ifdef __cplusplus
}
#endif

#endif /* HAXIAL_LIST_H */
