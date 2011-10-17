/* header guard generate by genheader.pl */
#ifndef _canvas_h_
#define _canvas_h_


struct block_t;
struct blockmap_t;
struct shapebuf_t;
struct ui_t;

struct canvas_t {
  struct SDL_Surface* sfc;
};

struct canvas_t* create_canvas(int w, int h);
struct canvas_t* load_image(const char* path);
void clear_canvas(struct canvas_t* canvas, unsigned long color);
void destroy_canvas(struct canvas_t* canvas);
void blit_canvas(struct canvas_t* dest, struct canvas_t* src, int x, int y);

void blit_ui(struct canvas_t* canvas, int x, int y);

void draw_block(struct canvas_t* cvs, const struct block_t* blk,
                int x, int y, int sz, unsigned long outln, int alpha);
void draw_sb(struct canvas_t* cvs, const struct shapebuf_t* sb,
             int x, int y, int sz, unsigned long outline, int alpha);
void draw_bm(struct canvas_t* cvs, const struct blockmap_t* bm,
             int x, int y, int sz, unsigned long outline, int alpha);
void draw_grid(struct canvas_t* cvs, const struct blockmap_t* bm,
               int x, int y, int sz, unsigned long color);
void draw_box(struct canvas_t* cvs, int x, int y, int w, int h,
               unsigned long outln, unsigned long fill);
void draw_shape(struct canvas_t* cvs, int x, int y, int sz, int shp,
                unsigned long outln, int alpha);


#endif /* _canvas_h_ */
