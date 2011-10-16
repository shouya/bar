/* header guard generate by genhead.pl */
#ifndef _blockmap_h_
#define _blockmap_h_

struct block_t {
  unsigned char occupied;
  unsigned char shape;
  unsigned char flags;
};

struct blockmap_t {
  int mapw, maph;
  block_t** buf;
};


struct shapebuf_t {
  int w, h;
  unsigned char** buf;
};

#endif /* _blockmap_h_ */
