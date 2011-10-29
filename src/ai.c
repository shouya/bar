#include <ai.h>

int ai_step(struct ai_t* ai, struct blockmap_t* bm, struct shapebuf_t* sb) {
  int rotated = 0, moved = 0;

  if (sb->rotate != ai->rotate) {
    soft_rotate_sb(sb, 1, bm, 2);
    rotated = 1;
  }

  if (sb->x != ai->move) {
  }

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




void ai_do(struct ai_t* ai, struct blockmap_t* bm, struct shapebuf_t* sb);
