/* header guard generate by genheader.pl */
#ifndef _queue_h_
#define _queue_h_

/* infinte queue random filling */

struct queue_t {
  int* queue;
  int size;
  int capacity;
  int range;
};

struct queue_t* create_queue(int range, int capacity);
int shift_queue(struct queue_t* q);



#endif /* _queue_h_ */
