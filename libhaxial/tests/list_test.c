/**
 * @file list_test.c
 * @brief Tests for intrusive linked list
 */

#include "haxial/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Test data structure
struct TestData {
    int value;
    HxListNode node;
};

static void test_init(void) {
    printf("Testing hx_list_init...\n");
    
    HxList list;
    hx_list_init(&list);
    
    assert(hx_list_is_empty(&list));
    assert(hx_list_count(&list) == 0);
    assert(hx_list_first(&list) == NULL);
    assert(hx_list_last(&list) == NULL);
    
    printf("  PASS\n");
}

static void test_append(void) {
    printf("Testing hx_list_append...\n");
    
    HxList list;
    hx_list_init(&list);
    
    struct TestData data1 = { .value = 1 };
    struct TestData data2 = { .value = 2 };
    struct TestData data3 = { .value = 3 };
    
    hx_list_append(&list, &data1.node);
    assert(hx_list_count(&list) == 1);
    assert(hx_list_first(&list) == &data1.node);
    assert(hx_list_last(&list) == &data1.node);
    
    hx_list_append(&list, &data2.node);
    assert(hx_list_count(&list) == 2);
    assert(hx_list_first(&list) == &data1.node);
    assert(hx_list_last(&list) == &data2.node);
    
    hx_list_append(&list, &data3.node);
    assert(hx_list_count(&list) == 3);
    assert(hx_list_first(&list) == &data1.node);
    assert(hx_list_last(&list) == &data3.node);
    
    printf("  PASS\n");
}

static void test_prepend(void) {
    printf("Testing hx_list_prepend...\n");
    
    HxList list;
    hx_list_init(&list);
    
    struct TestData data1 = { .value = 1 };
    struct TestData data2 = { .value = 2 };
    struct TestData data3 = { .value = 3 };
    
    hx_list_prepend(&list, &data1.node);
    assert(hx_list_count(&list) == 1);
    assert(hx_list_first(&list) == &data1.node);
    assert(hx_list_last(&list) == &data1.node);
    
    hx_list_prepend(&list, &data2.node);
    assert(hx_list_count(&list) == 2);
    assert(hx_list_first(&list) == &data2.node);
    assert(hx_list_last(&list) == &data1.node);
    
    hx_list_prepend(&list, &data3.node);
    assert(hx_list_count(&list) == 3);
    assert(hx_list_first(&list) == &data3.node);
    assert(hx_list_last(&list) == &data1.node);
    
    printf("  PASS\n");
}

static void test_remove(void) {
    printf("Testing hx_list_remove...\n");
    
    HxList list;
    hx_list_init(&list);
    
    struct TestData data1 = { .value = 1 };
    struct TestData data2 = { .value = 2 };
    struct TestData data3 = { .value = 3 };
    
    hx_list_append(&list, &data1.node);
    hx_list_append(&list, &data2.node);
    hx_list_append(&list, &data3.node);
    
    // Remove middle
    hx_list_remove(&list, &data2.node);
    assert(hx_list_count(&list) == 2);
    assert(hx_list_first(&list) == &data1.node);
    assert(hx_list_last(&list) == &data3.node);
    assert(data1.node.next == &data3.node);
    assert(data3.node.prev == &data1.node);
    
    // Remove head
    hx_list_remove(&list, &data1.node);
    assert(hx_list_count(&list) == 1);
    assert(hx_list_first(&list) == &data3.node);
    assert(hx_list_last(&list) == &data3.node);
    
    // Remove last
    hx_list_remove(&list, &data3.node);
    assert(hx_list_count(&list) == 0);
    assert(hx_list_is_empty(&list));
    
    printf("  PASS\n");
}

static void test_iteration(void) {
    printf("Testing list iteration...\n");
    
    HxList list;
    hx_list_init(&list);
    
    struct TestData data1 = { .value = 1 };
    struct TestData data2 = { .value = 2 };
    struct TestData data3 = { .value = 3 };
    
    hx_list_append(&list, &data1.node);
    hx_list_append(&list, &data2.node);
    hx_list_append(&list, &data3.node);
    
    // Test forward iteration
    HxListNode *node;
    int expected = 1;
    HX_LIST_FOR_EACH(&list, node) {
        struct TestData *data = HX_LIST_ENTRY(node, struct TestData, node);
        assert(data->value == expected);
        expected++;
    }
    assert(expected == 4);
    
    // Test safe iteration with removal
    HxListNode *tmp;
    HX_LIST_FOR_EACH_SAFE(&list, node, tmp) {
        struct TestData *data = HX_LIST_ENTRY(node, struct TestData, node);
        if (data->value == 2) {
            hx_list_remove(&list, node);
        }
    }
    assert(hx_list_count(&list) == 2);
    
    printf("  PASS\n");
}

static void test_list_entry(void) {
    printf("Testing HX_LIST_ENTRY macro...\n");
    
    HxList list;
    hx_list_init(&list);
    
    struct TestData data = { .value = 42 };
    hx_list_append(&list, &data.node);
    
    HxListNode *node = hx_list_first(&list);
    struct TestData *retrieved = HX_LIST_ENTRY(node, struct TestData, node);
    
    assert(retrieved == &data);
    assert(retrieved->value == 42);
    
    printf("  PASS\n");
}

int main(void) {
    printf("Running list tests...\n\n");
    
    test_init();
    test_append();
    test_prepend();
    test_remove();
    test_iteration();
    test_list_entry();
    
    printf("\nAll list tests passed!\n");
    return 0;
}
