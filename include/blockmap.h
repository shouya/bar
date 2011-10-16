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
  int w, h, shape, rotate, x, y;
  unsigned char** buf;
};


struct blockmap_t* create_blockmap(int w, int h);
void destory_blockmap(struct blockmap_t* bm);

struct shapebuf_t* create_shapebuf(int shape);
void destory_shapebuf(struct shapebuf_t* sb);


void rotate_sb(struct shapebuf_t* sb, int dir);
void move_sb(struct shapebuf_t* sb, int offx, int offy);

int check_sb(const struct blockmap_t* bm, const srtuct shapebuf_t* sb);
void merge_sb(struct blockmap_t* bm, const struct shapebuf_t* sb);

int check_bm_lines(const struct blockmap_t* bm, int* lnbuf, int bufsz);
void kill_bm_lines(struct blockmap_t* tm, const int* lnbuf, int len)
#endif /* _blockmap_h_ */
