#ifndef FIFO_HEADER
#define FIFO_HEADER

typedef struct queue queue_t;
typedef struct node node_t;

typedef struct {
    queue_t* (* const create)();
    void (* const delete)(queue_t* );
    void (* const enqueue)(queue_t*, void* );
    void* (* const dequeue)(queue_t*);
} _queue_api;

extern _queue_api const queue_api;

#endif
