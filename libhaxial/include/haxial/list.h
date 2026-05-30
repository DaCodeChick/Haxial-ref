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
 * 
 * This struct is NOT opaque - you can directly access next/prev pointers.
 */
typedef struct TListNode {
    struct TListNode *next;
    struct TListNode *prev;
} TListNode;

/**
 * List head structure (opaque - use provided functions)
 * 
 * For stack allocation, use HX_LIST_SIZE with aligned storage:
 *   alignas(max_align_t) char buf[HX_LIST_SIZE];
 *   TList *list = (TList *)buf;
 */
typedef struct TList TList;

/**
 * Size of TList structure for stack allocation
 */
#define HX_LIST_SIZE (sizeof(void*) * 2 + sizeof(size_t))

/**
 * Initialize a list
 */
void hx_list_init(TList *list);

/**
 * Check if list is empty
 */
bool hx_list_is_empty(const TList *list);

/**
 * Get number of elements in list
 */
size_t hx_list_count(const TList *list);

/**
 * Prepend node to front of list
 */
void hx_list_prepend(TList *list, TListNode *node);

/**
 * Append node to end of list
 */
void hx_list_append(TList *list, TListNode *node);

/**
 * Remove node from list
 */
void hx_list_remove(TList *list, TListNode *node);

/**
 * Pop head node from list (remove and return first node)
 * 
 * @param list List to pop from
 * @return First node, or NULL if list is empty
 */
TListNode* hx_list_pop_head(TList *list);

/**
 * Get first node (or NULL if empty)
 */
TListNode* hx_list_first(const TList *list);

/**
 * Get last node (or NULL if empty)
 */
TListNode* hx_list_last(const TList *list);

/**
 * Get next node (or NULL if at end)
 */
TListNode* hx_list_next(const TListNode *node);

/**
 * Get previous node (or NULL if at start)
 */
TListNode* hx_list_prev(const TListNode *node);

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
 * @param list Pointer to TList
 * @param node TListNode* variable for iteration
 */
#define HX_LIST_FOR_EACH(list, node) \
    for ((node) = hx_list_first(list); (node) != NULL; (node) = (node)->next)

/**
 * Iterate over list safely (allows removal during iteration)
 * 
 * @param list Pointer to TList
 * @param node TListNode* variable for iteration
 * @param tmp TListNode* variable for temporary storage
 */
#define HX_LIST_FOR_EACH_SAFE(list, node, tmp) \
    for ((node) = hx_list_first(list), (tmp) = (node) ? (node)->next : NULL; \
         (node) != NULL; \
         (node) = (tmp), (tmp) = (node) ? (node)->next : NULL)

#ifdef __cplusplus
}
#endif

#endif /* HAXIAL_LIST_H */
