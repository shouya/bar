/* header guard generate by genheader.pl */
#ifndef _queue_h_
#define _queue_h_

/* infinte queue random filling */

struct queue_t {
  int* queue;
  int capacity;
  int range;
  int* randbuf;
  int randptr;
};

struct queue_t* create_queue(int range, int capacity);
void destroy_queue(struct queue_t* queue);
int shift_queue(struct queue_t* q);



#endif /* _queue_h_ */
