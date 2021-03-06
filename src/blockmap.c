#include <string.h>
#include <stdlib.h>

#include <blockmap.h>

#include <config.h>
#include <shape.h>


struct blockmap_t* create_blockmap(int w, int h) {
  struct blockmap_t* bm;
  bm = calloc(1, sizeof(struct blockmap_t));
  bm->buf = calloc(w*h, sizeof(struct block_t));
  bm->w = w; bm->h = h;
  return bm;
}

void destroy_blockmap(struct blockmap_t* bm) {
  free(bm->buf);
  free(bm);
}

struct blockmap_t* clone_blockmap(const struct blockmap_t* bm) {
  struct blockmap_t* new_bm;
  new_bm = create_blockmap(bm->w, bm->h);
  memcpy(new_bm->buf, bm->buf, bm->w * bm->h * sizeof(struct block_t));

  return new_bm;
}
void copy_blockmap(struct blockmap_t* dest, const struct blockmap_t* src) {
  if (dest->w != src->w || dest->h != src->h) return;
  memcpy(dest->buf, src->buf, src->w * src->h * sizeof(struct block_t));
}


struct shapebuf_t* create_shapebuf(int shape) {
  struct shapebuf_t* sb;
  sb = calloc(1, sizeof(struct shapebuf_t));
  reset_shapebuf(sb, shape);
  return sb;
}

void reset_shapebuf(struct shapebuf_t* sb, int new_shape) {
  int i, j;
  if (new_shape == -1) {
    sb->w = sb->h = 0;
    if (sb->buf) free(sb->buf);
    sb->buf = 0;
    sb->shape = -1;
    return;
  }
  sb->w = g_shps[new_shape].w;
  sb->h = g_shps[new_shape].h;
  if (sb->buf) free(sb->buf);
  sb->buf = calloc(sb->w * sb->h, sizeof(unsigned char));
  sb->x = sb->y = sb->rotate = 0;
  sb->shape = new_shape;
  for (i = 0; i != sb->w; ++i) {
    for (j = 0; j != sb->h; ++j) {
      sb->buf[j*sb->w+i] = g_shps[new_shape].pix[j][i];
    }
  }
}

void destroy_shapebuf(struct shapebuf_t* sb) {
  free(sb->buf);
  free(sb);
}

struct shapebuf_t* clone_shapebuf(const struct shapebuf_t* sb) {
  struct shapebuf_t* new_sb;
  new_sb = calloc(1, sizeof(struct shapebuf_t));
  new_sb->w=sb->w; new_sb->h=sb->h; new_sb->x=sb->x; new_sb->y=sb->y;
  new_sb->rotate=sb->rotate; new_sb->shape=sb->shape;
  new_sb->buf = calloc(sb->w * sb->h, sizeof(unsigned char));
  memcpy(new_sb->buf, sb->buf, sizeof(unsigned char)* sb->w * sb->h);

  return new_sb;
}

void copy_shapebuf(struct shapebuf_t* dest, const struct shapebuf_t* src) {
  if (dest->w != src->w || dest->h != src->h) return;
  memcpy(dest->buf, src->buf, sizeof(unsigned char)* src->w * src->h);
  dest->w=src->w; dest->h=src->h; dest->x=src->x; dest->y=src->y;
  dest->rotate=src->rotate; dest->shape=src->shape;
}


void soft_reset_sb(struct shapebuf_t* sb, int new_shape,
                   const struct blockmap_t* bm, int lim) {
  int n = 0;
  reset_shapebuf(sb, new_shape);

  if (sb->shape == -1) return;

  sb->x = (bm->w - sb->w) / 2;

  do {
    if (check_sb(bm, sb) != 0) {
      if (n < 0) {
        n = 1 - n;
      } else {
        n = -1 - n;
      }
      sb->x += n;
    } else {
      break;
    }
  } while (abs(n) < lim);

}

void clear_blockmap(struct blockmap_t* bm) {
  int i, j;
  for (i = 0; i != bm->w; ++i) {
    for (j = 0; j != bm->h; ++j) {
      memset(&bm->buf[j*bm->w+i], 0, sizeof(struct block_t));
    }
  }
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
    int tmp = sb->w;
    sb->w = sb->h; sb->h = tmp;
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

void soft_rotate_sb(struct shapebuf_t* sb, int dir,
                    const struct blockmap_t* bm, int lim) {
  int n = 0, differ, ox;

  ox = sb->x;
  differ = sb->w;
  rotate_sb(sb, dir);
  differ -= sb->w;
  if (differ % 2) {
    if (differ < 0) differ -= 1;
    else differ += 1;
  }
  differ /= 2;
  sb->x += differ;

  if (sb->x < 0) {
    sb->x = 0;
  }
  if (sb->x >= bm->w) {
    sb->x = bm->w-sb->w;
  }

  do {
    if (check_sb(bm, sb) != 0) {
      if (n < 0) {
        n = 1 - n;
      } else {
        n = -1 - n;
      }
      sb->x += n;
    } else {
      return;
    }
  } while (abs(n) < lim);

  if (check_sb(bm, sb) != 0) {
    sb->x = ox;
    rotate_sb(sb, -dir);
    return;
  }
}


void move_sb(struct shapebuf_t* sb, int offx, int offy) {
  sb->x += offx;
  sb->y += offy;
}

void soft_move_sb(struct shapebuf_t* sb, int offx, int offy,
                  struct blockmap_t* bm) {
  move_sb(sb, offx, offy);
  if (check_sb(bm, sb) != 0) {
    move_sb(sb, -offx, -offy);
  }
}

void hard_drop_sb(struct shapebuf_t* sb, const struct blockmap_t* bm) {
  while (check_sb(bm, sb) == 0) {
    move_sb(sb, 0, 1);
  }
  move_sb(sb, 0, -1);
}


int check_sb(const struct blockmap_t* bm, const struct shapebuf_t* sb) {
  int i, j, x, y;
  if (sb->shape == -1) return -3;
  for (j = 0; j != sb->h; ++j) {
    for (i = 0; i != sb->w; ++i) {
      if (sb->buf[j*sb->w+i]) {
        x = i + sb->x;
        y = j + sb->y;
        if (x<0 || x>=bm->w || y<0 || y>=bm->h) {
          return -1;
        } else if (bm->buf[y*bm->w+x].occupied) {
          return -2;
        }
      } /* if sb->buf[j][i] */
    } /* for i */
  }   /* for j */
  return 0;
}

void merge_sb(struct blockmap_t* bm, const struct shapebuf_t* sb) {
  int i, j, x, y;
  if (check_sb(bm, sb) != 0) return;

  for (j = 0; j != sb->h; ++j) {
    for (i = 0; i != sb->w; ++i) {
      if (sb->buf[j*sb->w+i]) {
        x = i + sb->x;
        y = j + sb->y;
        bm->buf[y*bm->w+x].occupied = 1;
        bm->buf[y*bm->w+x].shape = sb->shape;
      } /* if sb->buf[j][i] */
    } /* for i */
  }   /* for j */
}

int check_bm_lines(const struct blockmap_t* bm, int* lnbuf, int bufsz) {
  int i, j, fullline, num = 0;
  for (j = bm->h-1; j >= 0; --j) {
    fullline = 1;
    for (i = 0; i != bm->w; ++i) {
      if (!bm->buf[j*bm->w+i].occupied) {
        fullline = 0;
        break;
      }
    }
    if (fullline) {
      ++num;
      if (bufsz != 0) {
        lnbuf[num-1] = j;
        if (num >= bufsz) {
          return bufsz;
        }
      }
    }
  }
  return num;
}

void kill_bm_lines(struct blockmap_t* bm, int* lnbuf, int len) {
  int n = 0, j, i;

  if (g_cfg.ctrl->kill_bm_lines) {
    (*g_cfg.ctrl->kill_bm_lines)(bm, lnbuf, len);
    return;
  }

  for (; n != len; ++n) {
    for (j = lnbuf[n]-1; j >= 0; --j) {
      memcpy(&(bm->buf[(j+1)*bm->w]), &(bm->buf[j*bm->w]),
             sizeof(struct block_t) * bm->w);
    }
    for (i = n+1; i != len; ++i) {
      if (lnbuf[i] < lnbuf[n]) {
        lnbuf[i] += 1;
      }
    } /* for each line in linebuf */
  } /* for each line above */
}


void set_ghost(const struct blockmap_t* bm,
               const struct shapebuf_t* sb,
               struct shapebuf_t* ghost) {
  if (ghost->w != sb->w || ghost->h != sb->h) {
    free(ghost->buf);
    ghost->buf = calloc(sb->w * sb->h, sizeof(unsigned char));
  }

  copy_shapebuf(ghost, sb);
  hard_drop_sb(ghost, bm);
}

void swap_hold(int* holdbuf, struct shapebuf_t* sb, int(*getnext)(void),
               const struct blockmap_t* bm, int lim) {
  int swp = sb->shape;
  if (*holdbuf == -1) {
    *holdbuf = (*getnext)();
  }
  if (bm != NULL) {
    soft_reset_sb(sb, *holdbuf, bm, lim);
  } else {
    reset_shapebuf(sb, *holdbuf);
  }
  *holdbuf = swp;
}

int steady_sb(struct blockmap_t* bm, struct shapebuf_t* sb,
              int nxtshp, int lim) {
  int lnbuf[SHAPE_H], num_clear;

  merge_sb(bm, sb);

  if ((num_clear = check_bm_lines(bm, lnbuf, SHAPE_H)) != 0) {
    kill_bm_lines(bm, lnbuf, num_clear);
  }

  soft_reset_sb(sb, nxtshp, bm, lim);

  if (check_sb(bm, sb) != 0) {
    return -1;
  } else {
    return 0;
  }
}

