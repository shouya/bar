/*
 * YaShou's tetris AI algorithm
 * Wrote at Oct 27, 2011
 */

#include <stdlib.h>

#include <blockmap.h>
#include <ai.h>


#define NOP 0
#define MV_LEFT 1
#define MV_RIGHT 2
#define RT_LEFT 3
#define RT_RIGHT 4
#define DROP 5

struct solution_t {
  int nums;
  struct solution_node_t {
    int rotate, x_pos;
    int holes;
    struct solution_node_t* next;
  } *head, *best;
};

static struct ai_t {
  struct solution_t* s;
} s_ai;

static struct solution_t* create_solution(void);
static void push_solution(struct solution_t* list,
                          int rotate, int x_pos, int holes);
static void destroy_solution(struct solution_t* solution);


static void calc_solutions(struct solution_t* solution,
                           const struct blockmap_t* org_bm,
                           const struct shapebuf_t* org_sb,
                           struct blockmap_t* test_bm,
                           struct shapebuf_t* test_sb);
static void best_solution(struct solution_t* solution);
static int calc_holes(const struct blockmap_t* bm);



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
  if (sb->x < ai->s->best->x_pos) {
    move_sb(sb, 1, 0);
    return AI_MOVE_RIGHT;
  } else if (sb->x > ai->s->best->x_pos) {
    move_sb(sb, -1, 0);
    return AI_MOVE_LEFT;
  }

  if (sb->rotate < ai->s->best->rotate) {
    rotate_sb(sb, 1);
    return AI_ROTATE_RIGHT;
  } else if (sb->rotate > ai->s->best->rotate) {
    rotate_sb(sb, -1);
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
                          int rotate, int x_pos, int holes) {
  struct solution_node_t* new_node = calloc(1, sizeof(struct solution_node_t));

  if (list->head == NULL) {
    list->head = new_node;
  } else {
    new_node->next = list->head;
    list->head = new_node;
  }

  new_node->rotate = rotate;
  new_node->x_pos = x_pos;
  new_node->holes = holes;

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
  int x_pos, rotate, holes, rl_i;

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

      holes = calc_holes(test_bm);

      push_solution(solution, rotate, x_pos, holes);
    }
  }
}

static void best_solution(struct solution_t* solution) {
  int least_holes = 1<<12; /* a big number(4096) */
  struct solution_node_t* least_node = NULL, *iterator = solution->head;

  while (iterator) {
    if (iterator->holes < least_holes) {
      least_holes = iterator->holes;
      least_node = iterator;
    }
    iterator = iterator->next;
  }

  solution->best = least_node;

  printf("best: x: %d, rotate: %d, holes: %d\n", least_node->x_pos, least_node->rotate, least_node->holes);
}

static int calc_holes(const struct blockmap_t* bm) {
  int top = bm->h, finish = 0, j, i, holes = 0;

  /* get the top of blocks */
  for (j = 0; j != bm->h; ++j) {
    for (i = 0; i != bm->w; ++i) {
      if (bm->buf[j*bm->w+i].occupied) {
        top = j;
        finish = 1;
        break;
      }
    }
    if (finish) break;
  }

  for (j = top; j != bm->h; ++j) {
    for (i = 0; i != bm->w; ++i) {
      if (!bm->buf[j*bm->w+i].occupied) {
        ++holes;
      }
    }
  }

  return holes;
}


