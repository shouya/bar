/*
 * This AI arglothm wrote by Pierre Dellacherie
 * I adapted it from C++ and into Bar.
 *
 * Author: Pierre Dellacherie
 * Year: 2003
 * Adapte date: October 29, 2011
 */
#include <math.h>
#include <string.h>

#include <ai.h>
#include <blockmap.h>

static struct ai_t {
  int height, rotate, move, priority;
  double rating;
} s_ai;


static void private_evalute(const struct blockmap_t* bm,
                            const struct shapebuf_t* sb,
                            double* rating, int* priority);
static double landing_height(const struct shapebuf_t* sb);
static int pile_height(const struct blockmap_t* bm);
static int completed_rows(const struct blockmap_t* bm);
static int count_transition_for_row(const struct blockmap_t* bm, int y);
static int count_transition_for_column(const struct blockmap_t* bm, int x);
static int buried_holes_for_column(const struct blockmap_t* bm, int x);
static int all_wells_for_column(const struct blockmap_t* bm, int x);
static int blanks_down_before_blocked(
  const struct blockmap_t* bm, int x, int topY
);

struct ai_t* ai_calc(struct blockmap_t* bm, struct shapebuf_t* sb,
                     int flags, void* data) {
  static int rotate_list[] = {0,1,2,3};
  int x_pos, rotate, rl_i, priority;
  double rating;
  struct blockmap_t* test_bm = clone_blockmap(bm);
  struct shapebuf_t* test_sb = clone_shapebuf(sb);

  memset(&s_ai, 0, sizeof(s_ai));

  for (rl_i = 0; rl_i != sizeof(rotate_list)/sizeof(*rotate_list); ++rl_i) {
    rotate = rotate_list[rl_i];

    for (x_pos = 0; x_pos != bm->w; ++x_pos) {
      copy_blockmap(test_bm, bm);
      copy_shapebuf(test_sb, sb);

      /* rotate and move to special position */
      while (test_sb->rotate != rotate) rotate_sb(test_sb, 1);
      if (test_sb->x < x_pos) {
        while (test_sb->x < x_pos) move_sb(test_sb, 1, 0);
      } else {
        while (test_sb->x > x_pos) move_sb(test_sb, -1, 0);
      }

      if (check_sb(test_bm, test_sb) != 0) continue;

      hard_drop_sb(test_sb, test_bm);

      private_evalute(bm, sb, &rating, &priority);


      if (rating > s_ai.rating ||
          ((rating == s_ai.rating) && (priority > s_ai.priority))) {
        s_ai.priority = priority;
        s_ai.rating = rating;
        s_ai.height = test_sb->y;
        s_ai.move = x_pos;
        s_ai.rotate = rotate;
      }

    }
  }

  puts("");

  destroy_blockmap(test_bm);
  destroy_shapebuf(test_sb);

  return &s_ai;
}

int ai_step(struct ai_t* ai, struct blockmap_t* bm, struct shapebuf_t* sb) {
  if (sb->rotate != ai->rotate) {
    soft_rotate_sb(sb, 1, bm, 2);
    return AI_ROTATE_RIGHT;
  }

  if (sb->x < ai->move) {
    soft_move_sb(sb, 1, 0, bm);
    return AI_MOVE_RIGHT;
  } else if (sb->x > ai->move) {
    soft_move_sb(sb, -1, 0, bm);
    return AI_MOVE_LEFT;
  }

  if (sb->rotate < ai->rotate) {
    soft_rotate_sb(sb, 1, bm, 2);
    return AI_ROTATE_RIGHT;
  } else if (sb->rotate > ai->rotate) {
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





static void private_evalute(const struct blockmap_t* bm,
                            const struct shapebuf_t* sb,
                            double* rating, int* priority) {
  int i, j;
  struct blockmap_t* temp_bm;
  double _landing_height = 0.0;
  int _completed_rows = 0;
  int _pile_height = 0;
  int _row_transition = 0;
  int _col_transition = 0;
  int _buried_holes = 0;
  int _board_wells = 0;
  int _eroded_piece_cells_metric = 0;

  temp_bm = clone_blockmap(bm);

  merge_sb(temp_bm, sb);

  *rating = 0.0;
  *priority = 0;

  _landing_height = bm->h - landing_height(sb);

  _completed_rows = completed_rows(temp_bm);
  _eroded_piece_cells_metric = pow(_completed_rows, 2);
  
  _pile_height = pile_height(temp_bm);
  _row_transition = 2 * (temp_bm->h - _pile_height);

  for (j = 0; j != temp_bm->h; ++j) {
    _row_transition += count_transition_for_row(temp_bm, j);
  }


  for (i = 0; i != temp_bm->w; ++i) {
    _col_transition += (count_transition_for_column(temp_bm, i));
    _buried_holes += (buried_holes_for_column(temp_bm, i));
    _board_wells += (all_wells_for_column(temp_bm, i));
  }



  *rating = 0.0;
  *rating += ((-1.0) * _landing_height);
  *rating += (( 1.0) * _eroded_piece_cells_metric);
  *rating += ((-1.0) * _row_transition);
  *rating += ((-1.0) * _col_transition);
  *rating += ((-4.0) * _buried_holes);
  *rating += ((-1.0) * _board_wells);

  printf("(%.1f, %.1f , %d, %d, %d, %d, %d)\n",
         rating,
         _landing_height,
         _eroded_piece_cells_metric,
         _row_transition,
         _col_transition,
         _buried_holes,
         _board_wells);


  *priority = 0;
  *priority += 100 * (int)abs((sb->x+sb->w)/2.0 - (bm->w/2.0));
  if (sb->x + sb->w < bm->w) *priority += 10;
  *priority -= (sb->rotate - 1);


  destroy_blockmap(temp_bm);
}



static double landing_height(const struct shapebuf_t* sb) {
  return (sb->h/2.0 + sb->y);
}

static int pile_height(const struct blockmap_t* bm) {
  int i, j;
  for (j = 0; j != bm->h; ++j) {
    for (i = 0; i != bm->w; ++i) {
      if (bm->buf[j*bm->w+i].occupied) return (bm->h - j);
    }
  }
  return (bm->h);
}

static int completed_rows(const struct blockmap_t* bm) {
  return (check_bm_lines(bm, NULL, 0));
}

static int count_transition_for_row(const struct blockmap_t* bm, int y) {
  int x, cell = 0, prev_cell = 0, transition = 0;
  for (x = 1; x != bm->w; ++x) {
    prev_cell = bm->buf[y*bm->w + (x-1)].occupied;
    cell = bm->buf[y*bm->w + x].occupied;

    if ((cell == 0 && prev_cell == 1) ||
        (cell == 1 && prev_cell == 0)) {
      ++transition;
    }
  }

  if (bm->buf[y*bm->w + 0].occupied == 0) ++transition;
  if (bm->buf[y*bm->w + (bm->w-1)].occupied == 0) ++transition;

  return transition;
}

static int count_transition_for_column(const struct blockmap_t* bm, int x) {
  int y, cell = 0, prev_cell = 0, transition = 0;
  for (y = 1; y != bm->h; ++y) {
    prev_cell = bm->buf[(y-1)*bm->w + x].occupied;
    cell = bm->buf[y*bm->w + x].occupied;

    if ((cell == 0 && prev_cell == 1) ||
        (cell == 1 && prev_cell == 0)) {
      ++transition;
    }
  }

  if (bm->buf[0*bm->w + x].occupied == 0) ++transition;
  if (bm->buf[(bm->h-1)*bm->w + x].occupied == 0) ++transition;

  return (transition);
}

static int buried_holes_for_column(const struct blockmap_t* bm, int x) {
  int y, holes = 0, enable = 0;
  for (y = 1; y != bm->h; ++y) {
    if (bm->buf[y*bm->w+x].occupied) {
      enable = 1;
    } else {
      if (enable) ++holes;
    }
  }
  return (holes);
}

static int blanks_down_before_blocked(
  const struct blockmap_t* bm, int x, int topY) {
  int y, blanks = 0;
  for (y = topY; y != bm->h; ++y) {
    if (bm->buf[y*bm->w+x].occupied) {
      return (blanks);
    } else {
      ++blanks;
    }
  }
  return (0);
}

static int all_wells_for_column(const struct blockmap_t* bm, int x) {
  int y, wells = 0, lwall = 0, rwall = 0;
  for (y = 0; y != bm->h; ++y) {
    if (x-1 < 0) {
      lwall = 1;
    } else {
      lwall = bm->buf[y*bm->w+(x-1)].occupied;
    }

    if (x+1 >= bm->w) {
      rwall = 1;
    } else {
      rwall = bm->buf[y*bm->w+(x+1)].occupied;
    }

    if (lwall && rwall) {
      wells += blanks_down_before_blocked(bm, x, y);
    }

  }

  return (wells);
}
