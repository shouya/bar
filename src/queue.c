#include <string.h>
#include <stdlib.h>

#include <queue.h>

int shift_randbuf(struct queue_t* q);

struct queue_t* create_queue(int range, int capacity) {
  struct queue_t* q;
  int i;
  if (capacity > range) { return 0; }

  q = calloc(1, sizeof(struct queue_t));
  q->capacity = capacity;
  q->range = range;
  q->queue = calloc(capacity, sizeof(int));
  q->randbuf = calloc(capacity, sizeof(int));
  q->randptr = 0;

  for (i = 0; i != q->capacity; ++i) {
    q->queue[i] = shift_randbuf(q);
  }

  return q;
}

void destroy_queue(struct queue_t* queue) {
  free(queue->queue);
  free(queue);
}


int shift_queue(struct queue_t* q) {
  int elm = q->queue[0], i;

  for (i = 1; i != q->capacity; ++i) {
    q->queue[i-1] = q->queue[i];
  }

  q->queue[q->capacity-1] = shift_randbuf(q);
  return elm;
}

int shift_randbuf(struct queue_t* q) {
  int i, rnd, cert;
  if (q->randptr > 0) {
    return q->randbuf[--q->randptr];
  }

  do {
    cert = 1;
    rnd = rand() % q->range;
    for (i = 0; i != q->randptr; ++i) {
      if (rnd == q->randbuf[i]) {
        cert = 0;
        break;
      }
    }
    if (cert) q->randbuf[q->randptr++] = rnd;
  } while (q->randptr != q->capacity);

  return shift_randbuf(q);
}

