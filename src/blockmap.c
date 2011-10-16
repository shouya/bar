#include <string.h>
#include <stdlib.h>

#include <blockmap.h>

#include <shape.h>


struct blockmap_t* create_blockmap(int w, int h) {
  struct blockmap_t* bm;
  bm = calloc(1, sizeof(struct blockmap_t));
  bm->buf = calloc(w*h, sizeof(struct block_t));
  bm->mapw = w; bm->maph = h;
  return bm;
}

void destory_blockmap(struct blockmap_t* bm) {
  free(bm->buf);
  free(bm);
}

struct shapebuf_t* create_shapebuf(int shape) {
  struct shapebuf_t* sb;
  sb = calloc(1, sizeof(struct shapebuf_t));
  sb->w = SHAPE_W;
  sb->h = SHAPE_H;
  sb->x = sb->y = b->rotate = 0;
  sb->buf = calloc(SHAPE_W*SHAPE_H, sizeof(unsigned char));
  memcpy(sb->buf, g_shps[shape].pix, SHAPE_W*SHAPE_H * sizeof(unsigned char));
  return sb;
}

void destory_shapebuf(struct shapebuf_t* sb) {
  free(sb->buf);
  free(sb);
}


void rotate_sb(struct shapebuf_t* sb, int dir) {
  static int xb[SHAPE_W*SHAPE_H];
  static int yb[SHAPE_W*SHAPE_H];
  int p = 0, i, j;

  if (dir != 1 && dir != -1 && dir != 0) return;

  for (i = 0; i != sb->w; ++i) {
    for (j = 0; j != sb->h; ++j) {
      if (sb->buf[j*sb->w+i]) {
        if (dir == 1) { /* clock wise 90' */
          xb[p] = sb->h - (j+1);
          yb[p] = i;
        } else if (dir == -1) { /* counter clockwise 90' */
          xb[p] = j;
          yb[p] = sb->w - (i+1);
        } else { /* just do nothing */
          xb[p] = i;
          yb[p] = j;
        }
        ++p;
      } /* if pixel */
    } /* for j */
  } /* for i */

  /* now swap w with h*/
  if (dir) {
    sb->w ^= sb->h ^= sb->w ^= sb->h;
  }

  /* set the new position of pixels */
  memset(sb->buf, 0, sb->w*sb->h);
  for (i = 0; i != p; ++i) {
    sb->buf[yb[i]*sb->w+xb[i]] = 1;
  }

  /* modify the rotate flag */
  sb->rotate += dir;
  if (sb->rotate == -1) sb->rotate = 3;
  if (sb->rotate == 4) sb->rotate = 0;
}

void move_sb(struct shapebuf_t* sb, int offx, int offy) {
  sb->x += offx;
  sb->y += offy;
}

int check_sb(const struct blockmap_t* bm, const srtuct shapebuf_t* sb) {
  int i, j, x, y;
  for (j = 0; j != sb->h; ++i) {
    for (i = 0; i != sb->w; ++i) {
      if (sb->buf[j*sb->w+i]) {
        x = i + sb->x;
        y = j + sb->y;
        if (x<0 || x>=bm->w || y<0 || y>=bm->h) return -1;
        if (bm->buf[y*bm->w+x].occupied) {
          return -1;
        }
      } /* if sb->buf[j][i] */
    } /* for i */
  }   /* for j */
  return 0;
}

void merge_sb(struct blockmap_t* bm, const struct shapebuf_t* sb) {
  int i, j, x, y;
  for (j = 0; j != sb->h; ++i) {
    for (i = 0; i != sb->w; ++i) {
      if (sb->buf[j*sb->w+i]) {
        x = i + sb->x;
        y = j + sb->y;
        bm->buf[y*bm->w+x].occupied = 1;
      } /* if sb->buf[j][i] */
    } /* for i */
  }   /* for j */
}

int check_bm_lines(const struct blockmap_t* bm, int* lnbuf, int bufsz) {
  int i, j, fullline, num = 0;
  for (j = bm->h-1; j >= 0; --j) {
    fulline = 1;
    for (i = 0; i != bm->w; ++i) {
      if (!bm->buf[j*bm->w+i].occupied) {
        fullline = 0;
        break;
      }
    }
    if (fullline) {
      lnbuf[num++] = j;
      if (num >= bufsz) {
        return -1;
      }
    }
  }
  return num;
}

void kill_bm_lines(struct blockmap_t* tm, const int* lnbuf, int len) {
  int n = 0, i, j;
  for (; n != len; ++n) {
    for (j = lnbuf[n]-1; j >= 0; ++j) {
      memcpy(tm->buf[j+1], bm->buf[j], sizeof(struct block_t) * bm->w);
    } /* for each line above */
  } /* for each line in linebuf */
}


