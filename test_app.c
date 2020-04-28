#include <stdio.h>
#include <stdlib.h>

#include "fifo.h"


extern _queue_api const queue_api;

typedef struct mydata {
    int x;
    char* y;

}mydata_t;

int main(){
    queue_t *test_queue = queue_api.create();
    if(test_queue == NULL ){
        printf("Failed to create queue\n");
        exit(1);
    }

    mydata_t data = {.x=1, .y= "test1"};

    mydata_t *out;
    queue_api.enqueue(test_queue, &data);
    out = (mydata_t *) queue_api.dequeue(test_queue);
    printf("x=%d,y=%s\n", out->x, out->y);
    queue_api.delete(test_queue);
    return 0;
}
