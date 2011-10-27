/* header guard generate by genheader.pl */
#ifndef _ai_h_
#define _ai_h_

struct blockmap_t;
struct shapebuf_t;

struct ai_t;

#define AI_ROTATE_LEFT  0x1
#define AI_ROTATE_RIGHT 0x2
#define AI_MOVE_LEFT    0x3
#define AI_MOVE_RIGHT   0x4
#define AI_DROP         0x5
#define AI_FREE         0x6

struct ai_t* ai_calc(struct blockmap_t* bm, struct shapebuf_t* sb,
                     int flags, void* data);
int ai_step(struct ai_t* ai, struct blockmap_t* bm, struct shapebuf_t* sb);
void ai_do(struct ai_t* ai, struct blockmap_t* bm, struct shapebuf_t* sb);



#endif /* _ai_h_ */
