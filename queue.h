/*
 * Queue header files
 */

#include "qtree.h"

#ifndef QTREE_SELF_IMPLEMENTATION_QUEUE_H
#define QTREE_SELF_IMPLEMENTATION_QUEUE_H

#endif //QTREE_SELF_IMPLEMENTATION_QUEUE_H

// structures
typedef struct qnode qnode_t;
struct qnode {
    qtnode_t* treeNode;
    qnode_t* next;
};
typedef struct queue queue_t;
struct queue {
    qnode_t* head;
    qnode_t* tail;
    int length;
};

// function prototypes
qnode_t* init_node(qtnode_t* treeNode);
queue_t* init_queue(qtnode_t* treeNode);
void enqueue(queue_t* q, qtnode_t* treeNode);
qtnode_t* dequeue(queue_t* q);
void print_queue(queue_t* q);
void free_queue(queue_t* q);