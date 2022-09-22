//
// Created by TheDuy on 18/09/2022.
//
#include "qtree.h"

#ifndef QTREE_SELF_IMPLEMENTATION_QUEUE_H
#define QTREE_SELF_IMPLEMENTATION_QUEUE_H

#endif //QTREE_SELF_IMPLEMENTATION_QUEUE_H

// structures
typedef struct qnode qnode_t;
struct qnode {
    node_t* treeNode;
    qnode_t* next;
};
typedef struct queue queue_t;
struct queue {
    qnode_t* head;
    qnode_t* tail;
    int length;
};

// function prototypes
qnode_t* init_node(node_t* treeNode);
queue_t* init_queue(node_t* treeNode);
void enqueue(queue_t* q, node_t* treeNode);
node_t* dequeue(queue_t* q);
void print_queue(queue_t* q);
void free_queue(queue_t* q);

//// structures
//typedef struct qnode qnode_t;
//struct qnode {
//    node_t* treeNode;
//    qnode_t* next;
//};
//typedef struct queue queue_t;
//struct queue {
//    qnode_t* head;
//    qnode_t* tail;
//    int length;
//};
//
//// function prototypes
//queue_t* init_queue(node_t* treeNode);
//void insert_queue(queue_t* q, node_t* treeNode);
//void print_queue(queue_t* q);