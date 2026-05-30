/**
 * @file list_test.c
 * @brief Tests for intrusive linked list
 */

#include "haxial/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdalign.h>

// Test data structure
struct TestData {
    int value;
    TListNode node;
};

static void test_basic(void) {
    printf("Testing basic list operations...\n");
    
    alignas(max_align_t) char list_buf[HX_LIST_SIZE];
    TList *list = (TList *)list_buf;
    hx_list_init(list);
    
    assert(hx_list_is_empty(list));
    assert(hx_list_count(list) == 0);
    
    struct TestData data1 = { .value = 1 };
    struct TestData data2 = { .value = 2 };
    struct TestData data3 = { .value = 3 };
    
    // Append
    hx_list_append(list, &data1.node);
    assert(hx_list_count(list) == 1);
    
    hx_list_append(list, &data2.node);
    hx_list_append(list, &data3.node);
    assert(hx_list_count(list) == 3);
    
    // Remove middle
    hx_list_remove(list, &data2.node);
    assert(hx_list_count(list) == 2);
    
    printf("  PASS\n");
}

static void test_iteration(void) {
    printf("Testing list iteration...\n");
    
    alignas(max_align_t) char list_buf[HX_LIST_SIZE];
    TList *list = (TList *)list_buf;
    hx_list_init(list);
    
    struct TestData data1 = { .value = 1 };
    struct TestData data2 = { .value = 2 };
    struct TestData data3 = { .value = 3 };
    
    hx_list_append(list, &data1.node);
    hx_list_append(list, &data2.node);
    hx_list_append(list, &data3.node);
    
    // Forward iteration
    TListNode *node;
    int expected = 1;
    HX_LIST_FOR_EACH(list, node) {
        struct TestData *data = HX_LIST_ENTRY(node, struct TestData, node);
        assert(data->value == expected);
        expected++;
    }
    assert(expected == 4);
    
    printf("  PASS\n");
}

static void test_pop_head(void) {
    printf("Testing list pop_head...\n");
    
    alignas(max_align_t) char list_buf[HX_LIST_SIZE];
    TList *list = (TList *)list_buf;
    hx_list_init(list);
    
    struct TestData data1 = { .value = 1 };
    struct TestData data2 = { .value = 2 };
    struct TestData data3 = { .value = 3 };
    
    hx_list_append(list, &data1.node);
    hx_list_append(list, &data2.node);
    hx_list_append(list, &data3.node);
    assert(hx_list_count(list) == 3);
    
    // Pop head
    TListNode *node = hx_list_pop_head(list);
    assert(node != NULL);
    struct TestData *data = HX_LIST_ENTRY(node, struct TestData, node);
    assert(data->value == 1);
    assert(hx_list_count(list) == 2);
    
    // Pop again
    node = hx_list_pop_head(list);
    assert(node != NULL);
    data = HX_LIST_ENTRY(node, struct TestData, node);
    assert(data->value == 2);
    assert(hx_list_count(list) == 1);
    
    // Pop last
    node = hx_list_pop_head(list);
    assert(node != NULL);
    data = HX_LIST_ENTRY(node, struct TestData, node);
    assert(data->value == 3);
    assert(hx_list_count(list) == 0);
    assert(hx_list_is_empty(list));
    
    // Pop from empty list
    node = hx_list_pop_head(list);
    assert(node == NULL);
    
    printf("  PASS\n");
}

int main(void) {
    printf("Running list tests...\n\n");
    
    test_basic();
    test_iteration();
    test_pop_head();
    
    printf("\nAll list tests passed!\n");
    return 0;
}
