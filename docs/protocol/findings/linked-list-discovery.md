# Linked List Implementation Discovery

## Overview
KDXServer uses a generic linked list implementation with a head structure that tracks metadata.

## LinkedListHead Structure

```c
struct LinkedListHead {
    void *head;           // +0x00: Pointer to first node
    void *tail;           // +0x04: Pointer to last node  
    uint link_offset;     // +0x08: Offset within node where link pointer(s) are
    uint count;           // +0x0c: Number of nodes in list
};
// Total: 16 bytes
```

### Design Pattern
This is a **generic intrusive linked list**:
- Nodes can be any structure type
- Link pointers are embedded within the node at a configurable offset
- Head structure stores metadata separate from nodes
- Same head structure works for singly or doubly-linked lists

### link_offset Field
The `link_offset` specifies where in each node the link pointer(s) are stored:
- **Singly-linked**: `node[link_offset]` = next pointer (4 bytes)
- **Doubly-linked**: `node[link_offset]` = next, `node[link_offset+4]` = prev (8 bytes)

## List Operation Functions

### list_prepend (singly-linked)
**Address**: 0x0806ed6c  
**Signature**: `void list_prepend(LinkedListHead *list, void *node)`

**Operation**: Add node to front of list
```c
void list_prepend(LinkedListHead *list, void *node) {
    void *old_head = list->head;
    node->next = old_head;              // At offset link_offset
    list->head = node;
    if (old_head == NULL) {
        list->tail = node;
    }
    list->count++;
}
```

**Use cases**:
- Stack-like LIFO operations
- Prepending to a list

### list_append (singly-linked)
**Address**: 0x0806edd8  
**Signature**: `void list_append(LinkedListHead *list, void *node)`

**Operation**: Add node to end of list
```c
void list_append(LinkedListHead *list, void *node) {
    node->next = NULL;                  // At offset link_offset
    if (list->tail == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;        // At offset link_offset
    }
    list->tail = node;
    list->count++;
}
```

**Use cases**:
- Queue-like FIFO operations
- Send queues
- Ordered lists

### dlist_append (doubly-linked)
**Address**: 0x0806efa4  
**Signature**: `void dlist_append(LinkedListHead *list, void *node)`

**Operation**: Add node to end of doubly-linked list
```c
void dlist_append(LinkedListHead *list, void *node) {
    node->next = NULL;                  // At offset link_offset
    node->prev = list->tail;            // At offset link_offset + 4
    if (list->tail == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;
    }
    list->tail = node;
    list->count++;
}
```

**Use cases**:
- Bidirectional traversal
- Efficient removal from middle
- Global socket/connection lists

## Usage Examples

### Send Queue in SocketInfo
From socket_send_or_queue analysis:
```c
// SocketInfo has send queue at offset +0x24
// Queue uses list_append to add entries
list_append(socket_info + 9, queue_entry);  // offset +0x24 = index [9]
```

### News List in ServerState
From server_initialize:
```c
// ServerState has news_list at offset +0x34
LinkedListHead news_list;
list_prepend(&server_state->news_list, news_item);
```

### Global Socket List
From FUN_0806efa4 usage:
```c
// Global doubly-linked list at DAT_080ddb20
dlist_append(&g_socket_list, socket_info);
```

## Node Structure Requirements

### For Singly-Linked Lists
Nodes must have a `next` pointer at the specified `link_offset`:
```c
struct MyNode {
    // ... other fields ...
    MyNode *next;    // At link_offset
};
```

### For Doubly-Linked Lists
Nodes must have both `next` and `prev` at the specified offset:
```c
struct MyNode {
    // ... other fields ...
    MyNode *next;    // At link_offset
    MyNode *prev;    // At link_offset + 4
};
```

## List Types Found

### 1. Send Queue (SocketInfo)
- **Type**: Singly-linked (list_append)
- **Location**: SocketInfo + 0x24 (field send_queue_head)
- **Purpose**: Queue of pending data to send on socket
- **Operations**: Append new entries, flush on write

### 2. News/Message List (ServerState)
- **Type**: Singly-linked (list_prepend)
- **Location**: ServerState + 0x34 (field news_list)
- **Purpose**: Server news/announcements
- **Operations**: Prepend new messages

### 3. Global Socket List
- **Type**: Doubly-linked (dlist_append)
- **Location**: Global at DAT_080ddb20
- **Purpose**: All active sockets in server
- **Operations**: Add/remove sockets, iterate all

## Missing Functions (Not Yet Found)

### Expected List Operations
- `list_remove` / `dlist_remove` - Remove node from list
- `list_pop_head` - Remove and return head node
- `list_pop_tail` - Remove and return tail node
- `list_find` - Search for node by criteria
- `list_foreach` - Iterate all nodes with callback
- `list_init` - Initialize empty list
- `list_clear` - Remove all nodes
- `list_is_empty` - Check if list has nodes

### Search Strategy
These functions likely exist near the ones we found (0x0806e000-0x0807000 range) or in nearby sections. Look for:
- Functions taking LinkedListHead* parameter
- Functions accessing [0], [1], [2], [3] indices
- Functions with loop patterns (for iteration)
- Functions that decrement count field

## Integration with ServerState

### Updated ServerState Field
```c
struct ServerState {
    SocketInfo *main_socket;              // +0x00
    // ... other fields ...
    LinkedListHead news_list;             // +0x34 (16 bytes)
    // ... more fields ...
};
```

The news_list is now properly typed as LinkedListHead instead of void*.

## Related Functions to Investigate

### Queue Entry Allocation
- `FUN_08098d3c` @ 0x08098d3c - allocate_send_queue_entry?
- Creates nodes for send queue

### Queue Flushing
- `FUN_08098ea0` @ 0x08098ea0 - flush_send_queue?
- Processes and removes entries from send queue

### Global List Management
- `DAT_080ddb20` - Global socket list head
- Used with dlist_append for socket registration

## Verification

### Confirmed by Code Analysis
- ✅ LinkedListHead structure layout (4 fields, 16 bytes)
- ✅ list_prepend implementation (singly-linked)
- ✅ list_append implementation (singly-linked)
- ✅ dlist_append implementation (doubly-linked)
- ✅ link_offset usage pattern
- ✅ Integration in ServerState
- ✅ Send queue in SocketInfo

### To Be Verified
- ⏳ Other list operation functions
- ⏳ Node structures for different list types
- ⏳ Initialization patterns
- ⏳ Removal/cleanup operations

## Summary

The KDXServer uses a well-designed generic intrusive linked list system:
- **Flexible**: Works with any node type
- **Efficient**: No allocations for list overhead
- **Typed**: LinkedListHead provides structure
- **Multiple variants**: Singly and doubly-linked support

This pattern is common in C systems programming (similar to Linux kernel lists).

## Next Steps
1. Find list removal/pop functions
2. Identify all list instances (global and per-struct)
3. Map node structures for each list type
4. Document complete list API
5. Apply LinkedListHead type to all list fields in structures
