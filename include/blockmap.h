/* header guard generate by genheader.pl */
#ifndef _blockmap_h_
#define _blockmap_h_

struct block_t {
  unsigned char occupied;
  unsigned char shape;
  unsigned char flags;
};

struct blockmap_t {
  int w, h;
  struct block_t* buf;
};


struct shapebuf_t {
  int w, h, shape, rotate, x, y;
  unsigned char* buf;
};


struct blockmap_t* create_blockmap(int w, int h);
void destroy_blockmap(struct blockmap_t* bm);
void clear_blockmap(struct blockmap_t* bm);
struct blockmap_t* clone_blockmap(const struct blockmap_t* bm);
void copy_blockmap(struct blockmap_t* dest, const struct blockmap_t* src);

struct shapebuf_t* create_shapebuf(int shape);
void reset_shapebuf(struct shapebuf_t* sb, int new_shape);
void soft_reset_sb(struct shapebuf_t* sb, int new_shape,
                   const struct blockmap_t* bm, int lim);
void destroy_shapebuf(struct shapebuf_t* sb);
struct shapebuf_t* clone_shapebuf(const struct shapebuf_t* sb);
void copy_shapebuf(struct shapebuf_t* dest, const struct shapebuf_t* src);


void rotate_sb(struct shapebuf_t* sb, int dir);
void move_sb(struct shapebuf_t* sb, int offx, int offy);
void soft_rotate_sb(struct shapebuf_t* sb, int dir,
                    const struct blockmap_t* bm, int lim);
void soft_move_sb(struct shapebuf_t* sb, int offx, int offy,
                  struct blockmap_t* bm);
void hard_drop_sb(struct shapebuf_t* sb, const struct blockmap_t* bm);

int check_sb(const struct blockmap_t* bm, const struct shapebuf_t* sb);
void merge_sb(struct blockmap_t* bm, const struct shapebuf_t* sb);

int check_bm_lines(const struct blockmap_t* bm, int* lnbuf, int bufsz);
void kill_bm_lines(struct blockmap_t* bm, int* lnbuf, int len);

void set_ghost(const struct blockmap_t* bm,
               const struct shapebuf_t* sb,
               struct shapebuf_t* ghost);

void swap_hold(int* holdbuf, struct shapebuf_t* sb, int(*getnext)(void),
               const struct blockmap_t* bm, int lim);

int steady_sb(struct blockmap_t* bm, struct shapebuf_t* sb,
              int nxtshp, int lim);


#endif /* _blockmap_h_ */
