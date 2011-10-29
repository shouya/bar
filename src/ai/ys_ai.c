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
static int calc_bm_score(const struct blockmap_t* bm);
static int calc_score(const struct blockmap_t* bm, int x, int y);



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
  int result = 0;
  while (result = ai_step(ai, bm, sb)) {
    if (result == AI_FREE || result == AI_DROP) return;
  }
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
  static int rotate_list[] = {0,1,2,3};
  int x_pos, rotate, score, rl_i;

  for (rl_i = 0; rl_i != sizeof(rotate_list)/sizeof(*rotate_list); ++rl_i) {
    rotate = rotate_list[rl_i];

    for (x_pos = 0; x_pos != org_bm->w; ++x_pos) {
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
      merge_sb(test_bm, test_sb);

      score = calc_bm_score(test_bm);

      push_solution(solution, rotate, x_pos, score);
    }
  }
}

static void best_solution(struct solution_t* solution) {
  int min_score = 1<<12; /* a big number(4096) */
  struct solution_node_t* min_node = NULL, *iterator = solution->head;

  while (iterator) {
    if (iterator->score < min_score) {
      min_score = iterator->score;
      min_node = iterator;
    }
    iterator = iterator->next;
  }

  solution->best = min_node;
}

static int calc_bm_score(const struct blockmap_t* bm) {
  int top = bm->h, score = 0, ln_killed, noempty = 0;
  int i, j;

  if ((ln_killed = check_bm_lines(bm, NULL, 0)) != 0) {
    score = -ln_killed * 30;
  }


  for (j = 0; j != bm->h; ++j) {
    for (i = 0; i != bm->w; ++i) {
      if (bm->buf[j*bm->w+i].occupied) {
        top = j;
        noempty = 1;
        break;
      }
    }
    if (noempty) break;
  }

  for (i = 0; i != bm->w; ++i) {
    noempty = 0;
    for (j = top; j != bm->h; ++j) {
      if (bm->buf[j*bm->w+i].occupied) {
        noempty = 1;
      }
    }
    if (!noempty) {
      score += 1;
    }
  }

  for (j = top; j != bm->h; ++j) {
    for (i = 0; i != bm->w; ++i) {
/*      if (bm->buf[j*bm->w+i].occupied) {*/
        score += calc_score(bm, i, j);
      }
    }
/*  }*/

  return score;
}


static int calc_score(const struct blockmap_t* bm, int x, int y) {
  int score = 0, cx = x, cy;

  if (bm->buf[y*bm->w+x].occupied) {
    for (cy = y; cy != bm->h; ++cy) {
      if (!bm->buf[cy*bm->w+cx].occupied) {
        ++score;
      }
    }
  } else {
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

  return score;
}
