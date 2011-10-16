#include <stdlib.h>

#include <queue.h>

static void regen_queue(struct queue_t* q);

struct queue_t* create_queue(int range, int capacity) {
  struct queue_t* q;
  if (capacity > range) { return 0; }
  q = calloc(1, sizeof(struct queue_t));
  q->capacity = capacity;
  q->range = range;
  q->queue = calloc(capacity, sizeof(int));
  q->size = 0;

  return q;
}

int shift_queue(struct queue_t* q) {
  if (q->size == 0) {
    regen_queue(q);
  }
  return q->queue[--q->size];
}

void regen_queue(struct queue_t* q) {
  int i, rnd, cert;
  do {
    cert = 1;
    rnd = rand() % q->range;
    for (i = 0; i != q->size; ++i) {
      if (rnd == q->queue[i]) {
        cert = 0;
        break;
      }
    }
    if (cert) q->queue[q->size++] = rnd;
  } while (q->size != q->capacity);
}

