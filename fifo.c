#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "fifo.h"

typedef struct node {
    void *data;
    void *next;
} node_t;

typedef struct queue {
    node_t *front;
    node_t *rear;
    pthread_mutex_t mutex;
    pthread_cond_t wakeup;
} queue_t;

static node_t* create_node(void*);
static queue_t* create();
static void delete(queue_t*);
static void enqueue(queue_t*, void*);
static void* dequeue(queue_t*);

static node_t* create_node(void* data){
    node_t* node = malloc(sizeof(node_t));
    if(node == NULL ){
        return NULL;
    }
    node->data = data;
    node->next = NULL;
}

static queue_t* create(){
    queue_t* queue = malloc(sizeof(queue_t));

    if (queue == NULL){
        return NULL;
    }

    queue->front = NULL;
    queue->rear = NULL;

    if(pthread_mutex_init(&queue->mutex, NULL) == 0 &&
            pthread_cond_init(&queue->wakeup, NULL) == 0){
        return queue;
    }

    free(queue);
    return NULL;
}

static void delete(queue_t* queue){
    if(queue->front != NULL){
        while(queue->front != queue->rear){
            node_t* temp = queue->front;
            queue->front = queue->front->next;
            free(temp);
        }
        free(queue->front);
        queue = NULL;
    }
}

static void enqueue(queue_t* queue, void* data){
    node_t* new_node = create_node(data);

    if(new_node == NULL){
        printf("\n");
       // return -1;
    }

    pthread_mutex_lock(&queue->mutex);

    //Queue is not created
    if(queue == NULL){
        queue_t* queue = create();
        if(queue == NULL){
        printf("\n");
        //    return -1;
        }
    }
    //Queue is empty
    if(queue->front == NULL && queue->rear == NULL ){
        queue->front = queue->rear = new_node;
    }else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }

    pthread_mutex_unlock(&queue->mutex);
    pthread_cond_signal(&queue->wakeup);

  //  return 0;
}

static void* dequeue(queue_t* queue) {

    pthread_mutex_lock(&queue->mutex);

    if(queue == NULL || queue->front == NULL || queue->rear == NULL)
        return NULL;

    node_t* temp = queue->front;
    void* data = queue->front->data;
    if(queue->front == queue->rear){
        queue->front = queue->rear = NULL;
    } else {
    queue->front =  queue->front->next;
    }
    free(temp);

    pthread_mutex_unlock(&queue->mutex);

    return data;
}

_queue_api const queue_api = {
    create,
    delete,
    enqueue,
    dequeue
};
