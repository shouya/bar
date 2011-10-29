/*
 * YaShou's tetris AI algorithm
 * Wrote at Oct 27, 2011
 */

#include <stdlib.h>

#include <blockmap.h>
#include <ai.h>


struct solution_t {
  int nums;
  struct solution_node_t {
    int rotate, x_pos;
    int score;
    struct solution_node_t* next;
  } *head, *best;
};

static struct ai_t {
  struct solution_t* s;
} s_ai;

static struct solution_t* create_solution(void);
static void push_solution(struct solution_t* list,
                          int rotate, int x_pos, int score);
static void destroy_solution(struct solution_t* solution);


static void calc_solutions(struct solution_t* solution,
                           const struct blockmap_t* org_bm,
                           const struct shapebuf_t* org_sb,
                           struct blockmap_t* test_bm,
                           struct shapebuf_t* test_sb);
static void best_solution(struct solution_t* solution);
static int calc_bm_score(struct blockmap_t* bm,
                         struct shapebuf_t* sb);
static int calc_touched_blocks(const struct blockmap_t* bm,
                               const struct shapebuf_t* sb);
static int calc_bm_top(const struct blockmap_t* bm,
                    const struct shapebuf_t* sb);
static int calc_holes(const struct blockmap_t* bm,
                      const struct shapebuf_t* sb,
                      int top);
static unsigned long block_walls(const struct blockmap_t* bm, int x, int y);
static int calc_ys_score(const struct blockmap_t* bm,
                         const struct shapebuf_t* sbp);
static int calc_score(const struct blockmap_t* bm,
                      int x, int y);


static int d1, d2, d3, d4;


struct ai_t* ai_calc(struct blockmap_t* bm, struct shapebuf_t* sb,
                     int flags, void* data) {
  struct solution_t* solution = create_solution();
  struct blockmap_t* test_bm = clone_blockmap(bm);
  struct shapebuf_t* test_sb = clone_shapebuf(sb);

  if (s_ai.s) destroy_solution(s_ai.s);

  calc_solutions(solution, bm, sb, test_bm, test_sb);
  best_solution(solution);

  s_ai.s = solution;

  destroy_blockmap(test_bm);
  destroy_shapebuf(test_sb);

  return &s_ai;
}

int ai_step(struct ai_t* ai, struct blockmap_t* bm, struct shapebuf_t* sb) {
  if (sb->rotate != ai->s->best->rotate) {
    soft_rotate_sb(sb, 1, bm, 2);
    return AI_ROTATE_RIGHT;
  }

  if (sb->x < ai->s->best->x_pos) {
    soft_move_sb(sb, 1, 0, bm);
    return AI_MOVE_RIGHT;
  } else if (sb->x > ai->s->best->x_pos) {
    soft_move_sb(sb, -1, 0, bm);
    return AI_MOVE_LEFT;
  }

  if (sb->rotate < ai->s->best->rotate) {
    soft_rotate_sb(sb, 1, bm, 2);
    return AI_ROTATE_RIGHT;
  } else if (sb->rotate > ai->s->best->rotate) {
    soft_rotate_sb(sb, -1, bm, 2);
    return AI_ROTATE_LEFT;
  }

  move_sb(sb, 0, 1);

  if (check_sb(bm, sb) == 0) {
    hard_drop_sb(sb, bm);
    return AI_DROP;
  } else {
    move_sb(sb, 0, -1);
    return AI_FREE;
  }
}

void ai_do(struct ai_t* ai, struct blockmap_t* bm, struct shapebuf_t* sb) {
  while (ai_step(ai, bm, sb) != AI_FREE) {}
}



static struct solution_t* create_solution(void) {
  return calloc(1, sizeof(struct solution_t));
}

static void push_solution(struct solution_t* list,
                          int rotate, int x_pos, int score) {
  struct solution_node_t* new_node = calloc(1, sizeof(struct solution_node_t));

  if (list->head == NULL) {
    list->head = new_node;
  } else {
    new_node->next = list->head;
    list->head = new_node;
  }

  new_node->rotate = rotate;
  new_node->x_pos = x_pos;
  new_node->score = score;

  ++list->nums;
}
static void destroy_solution(struct solution_t* solution) {
  struct solution_node_t* node = solution->head;
  while (node) {
    if (node) {
      solution->head = node;
      node = node->next;
      free(solution->head);
    }
  }
  free(solution);
}


static void calc_solutions(struct solution_t* solution,
                           const struct blockmap_t* org_bm,
                           const struct shapebuf_t* org_sb,
                           struct blockmap_t* test_bm,
                           struct shapebuf_t* test_sb) {
  static int rotate_list[] = {3,1,2,0};
  int x_pos, rotate, score, rl_i;

  for (rl_i = 0; rl_i != sizeof(rotate_list)/sizeof(*rotate_list); ++rl_i) {
    rotate = rotate_list[rl_i];

    d1=rotate;

    for (x_pos = 0; x_pos != org_bm->w; ++x_pos) {
      d2=x_pos;
      copy_blockmap(test_bm, org_bm);
      copy_shapebuf(test_sb, org_sb);

      /* rotate and move to special position */
      while (test_sb->rotate != rotate) rotate_sb(test_sb, 1);
      if (test_sb->x < x_pos) {
        while (test_sb->x < x_pos) move_sb(test_sb, 1, 0);
      } else {
        while (test_sb->x > x_pos) move_sb(test_sb, -1, 0);
      }

      if (check_sb(test_bm, test_sb) != 0) continue;

      hard_drop_sb(test_sb, test_bm);

      score = calc_bm_score(test_bm, test_sb);

/*      merge_sb(test_bm, test_sb);*/

      push_solution(solution, rotate, x_pos, score);
    }
  }
}

static void best_solution(struct solution_t* solution) {
  int max_score = -10000;
  struct solution_node_t* max_node = NULL, *iterator = solution->head;

  while (iterator) {
    if (iterator->score > max_score) {
      max_score = iterator->score;
      max_node = iterator;
    }
    iterator = iterator->next;
  }

  solution->best = max_node;
  printf("The best: %d, %d, %d\n", max_node->x_pos, max_node->rotate,
         max_node->score);
}

static int calc_bm_score(struct blockmap_t* bm, struct shapebuf_t* sb) {
  int top = 0, score = 0;
  int i, j;

  score += 2*(top = calc_bm_top(bm, sb));
  score -= 1*(calc_holes(bm, sb, top));
/*
  score += 1*(calc_touched_blocks(bm, sb));
*/
  score += 100*(check_bm_lines(bm, NULL, 0));

  score -= 1*(calc_ys_score(bm, sb));
  printf("%d\n", score);

  return score;
}

static int calc_ys_score(const struct blockmap_t* bm,
                         const struct shapebuf_t* sb) {
/*  char* col = calloc(sb->w, sizeof(char));*/
  int i, j, x, y, cx, cy, h, score = 0;
  struct blockmap_t* tbm = clone_blockmap(bm);
/*
  for (j = 0; j != sb->h; ++j) {
    for (i = 0; i != sb->w; ++i) {
      if (col[i]) continue;
      if (sb->buf[j*sb->w+i]) {
        for (h = j; h != sb->h; ++h) {
          x = sb->x + i; y = sb->y + h;
          if (!bm->buf[y*bm->w+x].occupied && !sb->buf[h*bm->w+i]) {
            ++score;
          }
        }
        col[i] = 1;
      }
    }
  }
  
  free(col);*/
  merge_sb(tbm, sb);
  for (j = sb->y; j != sb->h+sb->y; ++j) {
    for (i = sb->x; i != sb->w+sb->x; ++i) {
      x=i; y=j;
      if (tbm->buf[y*bm->w+x].occupied) continue;
      for (cx = x-1; cx != x+1; ++cx) {
        for (cy = y-1; cy != y+1; ++cy) {
          if (cx < 0 || cx >= bm->w || cy < 0 || cy >= bm->h) {
            score += 1;
          } else {
            score += tbm->buf[cy*bm->w+cx].occupied;
          }
        }
      }
    }
  }
  

  destroy_blockmap(tbm);
  printf("score: %d\t", score);
  return score;
}

static int calc_score(const struct blockmap_t* bm, int x, int y) {
  int score = 0, cx = x, cy;

  if (!bm->buf[y*bm->w+x].occupied) {
    for (cx = x-1; cx != x+1; ++cx) {
      for (cy = y-1; cy != y+1; ++cy) {
        if (cx < 0 || cx >= bm->w || cy < 0 || cy >= bm->h) { /* wall */
          score += 1;
        } else {
          score += bm->buf[cy*bm->w+cx].occupied;
        }
      }
    }
  } 
  for (cy = y; cy != bm->h; ++cy) {
    if (!bm->buf[cy*bm->w+cx].occupied) {
      ++score;
    }
  }

  return score;
}

static int calc_touched_blocks(const struct blockmap_t* bm,
                               const struct shapebuf_t* sb) {
  int i, j, x, y, rx, ry, bx, by, count = 0;
/*  struct blockmap_t* test_bm = clone_blockmap(bm);*/
  char* blocks = calloc((bm->w+2)*(bm->h+2), sizeof(char));
  char* p = blocks;
  

  for (i = 0; i != sb->w; ++i) {
    for (j = 0; j != sb->h; ++j) {
      if (!sb->buf[j*sb->w+i]) continue;
      x = i+sb->x; y = j+sb->y;

      for (rx = x-1; rx != x+2; ++rx) {
        for (ry = y-1; ry != y+2; ++ry) {
          bx = rx + 1; by = ry + 1;
          if (rx<0 || ry<0 || rx>=bm->w || ry>=bm->h) {
            blocks[by*(bm->w+2)+bx] = 1;
          }
          if (bm->buf[ry*bm->w+rx].occupied) {
            blocks[by*(bm->w+2)+bx] = 1;
          }
        }
      } /* for each related blocks */
    }
  } /* for each occupied block in shapebuf */

  while (p - blocks < (bm->w+2)*(bm->h+2)) {
    count += *p++;
  }/*
  for (i = -1; i != sb->w + 1; ++i) {
    for (j = -1; j != sb->h + 1; ++j) {
      x = i+sb->x; y = j+sb->y;
      if (test_bm->buf[y*bm->w+x].flags == 1) {
        ++count; continue;
      }
    }
    }*/

  printf("touched: %d of rotate: %d, x_pos: %d\t", count, d1, d2);

  free(blocks);

  return count;
}

static int calc_bm_top(const struct blockmap_t* bm,
                       const struct shapebuf_t* sb) {
  int i, j;
  for (j = 0; j != bm->h; ++j) {
    for (i = 0; i != bm->w; ++i) {
      if (bm->buf[j*bm->w+i].occupied) {
        if (sb->y > j) {
          return j;
        } else {
          return sb->y;
        }
      }
    }
  }
  return sb->y;
}
static int calc_holes(const struct blockmap_t* bm,
                      const struct shapebuf_t* sb,
                      int top) {
  int i, j, x, y, holes = 0;
  char* holemap = calloc((sb->w+2)*(sb->h+2), sizeof(char)), p;
/*
  for (j = top; j != bm->h; ++j) {
    for (i = 0; i != bm->w; ++i) {
      x = i - sb->x;  y = j - sb->y;
      if (bm->buf[j*bm->w+i].occupied) continue;
      if (x>=0 && x<sb->w && y>=0 && y<sb->h &&
          sb->buf[y*sb->w+x]); continue;
      ++holes;
    }
    }*/
  for (i = -1; i != sb->w + 1; ++i) {
    for (j = -1; j != sb->h + 1; ++j) {
      x = sb->x+i; y = sb->y+j;
      if (x<0||x>=bm->w||y<0||y>=bm->h) { ++holes; continue; }
      if (bm->buf[y*bm->w+x].occupied) continue;
      if (i>=0&&i<sb->w&&j>=0&&j<sb->h &&
          sb->buf[j*sb->w+i]) continue;
      ++holes;
    }
  }

  printf("h %d\t", holes);

  free(holemap);
  return holes;
}
/*
unsigned long block_walls(const struct blockmap_t* bm, int x, int y) {
  unsigned long flags = 0;
  if (x-1<0 || bm->buf[y*bm->w+(x-1)].occupied) flags |= WALL_LEFT;
  if (y-1<0 || bm->buf[(y-1)*bm->w+x].occupied) flags |= WALL_TOP;
  if (x+1>=bm->w || bm->buf[y*bm->w+(x+1)].occupied) flags |= WALL_RIGHT;
  if (y+1>=bm->h || bm->buf[(y+1)*bm->w+x].occupied) flags |= WALL_BOTTOM;
  return flags;
}
*/
