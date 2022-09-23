//
// Created by TheDuy on 18/09/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "queue.h"

/*
 * initialize queue
 */
queue_t* init_queue(qtnode_t* treeNode) {
    assert(treeNode);
    queue_t* q = (queue_t*) malloc(sizeof(queue_t));
    assert(q);
    q->head = init_node(treeNode);
    q->head->next = NULL;
    q->length = 1;
    q->tail = q->head;
    return q;
}

/*
 * initialize node
 */
qnode_t* init_node(qtnode_t* treeNode) {
    assert(treeNode);
    qnode_t* qnode = (qnode_t*) malloc(sizeof(qnode_t));
    assert(qnode);
    qnode->treeNode = treeNode;
    return qnode;
}

/*
 * insert to queue - enqueue
 */
void enqueue(queue_t* q, qtnode_t* treeNode) {
    assert(treeNode);
    assert(q);
    q->tail->next = init_node(treeNode);
    q->tail = q->tail->next;
    q->length = q->length+1;
}

/*
 * remove from queue - dequeue
 */
qtnode_t* dequeue(queue_t* q) {
    assert(q->head && q->tail);
    qtnode_t* node = q->head->treeNode;
    q->head = q->head->next;
    q->length = q->length-1;
    return node;
}

/*
 * freeing queue memory
 */
void free_queue(queue_t* q) {
    assert(q);
    qnode_t* tmp;
    for (int i=0; i < q->length; i++) {
        tmp = q->head;
        q->head = q->head->next;
        free(tmp);
    }
    free(q);
}

/*
 * print queue
 */
void print_queue(queue_t* q) {
    assert(q->head && q->tail);
    qnode_t* data = q->head;
    for (int i=0; i < q->length; i++) {
        printf("%Lf %Lf\n", data->treeNode->point->x, data->treeNode->point->y);
        data = data->next;
    }
}
