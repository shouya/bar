#include <SDL/SDL.h>

#include <canvas.h>

#include <ui.h>
#include <config.h>
#include <blockmap.h>
#include <shape.h>
#include <graphic.h>

#ifdef ALLOW_IMAGE
#include <SDL/image.h>
#endif /* ALLOW IMAGE */


struct canvas_t* create_canvas(int w, int h) {
  struct canvas_t* cvs;
  SDL_Surface* sfc = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
                                          0xff<<16, 0xff<<8, 0xff, 0);
  if (!sfc) {
    return NULL;
  }
  cvs = calloc(1, sizeof(struct canvas_t));
  cvs->sfc = sfc;
  return cvs;
}

void clear_canvas(struct canvas_t* canvas, unsigned long color) {
  SDL_FillRect(canvas->sfc, NULL, color);
}

struct canvas_t* load_image(const char* path) {
#ifdef ALLOW_IMAGE
  return 0;
#endif /* ALLOW_IMAGE */
  return 0;
}

void destroy_canvas(struct canvas_t* canvas) {
  if (!canvas) return;
  if (canvas->sfc) {
    SDL_FreeSurface(canvas->sfc);
  }
  free(canvas);
}

void blit_canvas(struct canvas_t* dest, struct canvas_t* src, int x, int y) {
  SDL_Rect dstrect;
  if (!(dest && src)) return;
  if (!(dest->sfc && src->sfc)) return;

  dstrect.x = x; dstrect.y = y;
  SDL_BlitSurface(src->sfc, NULL, dest->sfc, &dstrect);
}


void blit_ui(struct canvas_t* canvas, int x, int y) {
  SDL_Rect dstrect = {x, y};
  if (!canvas) return;
  if (!canvas->sfc) return;

  SDL_BlitSurface(canvas->sfc, NULL, g_ui->scr, &dstrect);
}

void draw_block(struct canvas_t* cvs, const struct block_t* blk,
                int x, int y, int sz,
                unsigned long outline, int alpha) {
  if (g_cfg.render->draw_block) {
    (*g_cfg.render->draw_block)(cvs, blk, x, y, sz, outline, alpha);
    return;
  }
  if (!blk->occupied) return;

  draw_box(cvs, x, y, sz, sz, outline,
            (g_shps[blk->shape].color | PACKA(alpha)));
}
void draw_sb(struct canvas_t* cvs, const struct shapebuf_t* sb,
             int x, int y, int sz, unsigned long outline, int alpha) {
  int i, j;
  struct block_t blk = {1, sb->shape, 0};
  for (j = 0; j != sb->h; ++j) {
    for (i = 0; i != sb->w; ++i) {
      if (sb->buf[j*sb->w+i]) {
        draw_block(cvs, &blk, x + (sb->x+i)*sz, y + (sb->y+j)*sz,
                   sz, outline, alpha);
      }
    }
  }
}

void draw_bm(struct canvas_t* cvs, const struct blockmap_t* bm,
             int x, int y, int sz, unsigned long outline, int alpha) {
  int i, j;
  for (i = 0; i != bm->w; ++i) {
    for (j = 0; j != bm->h; ++j) {
      if (bm->buf[j*bm->w+i].occupied) {
        draw_block(cvs, &(bm->buf[j*bm->w+i]),
                   x+i*sz, y+j*sz, sz, outline, alpha);
      }
    }
  }
}

void draw_grid(struct canvas_t* cvs, const struct blockmap_t* bm,
               int x, int y, int sz, unsigned long color) {
  int i, j;
  for (i = 0; i != bm->w; ++i) {
    for (j = 0; j != bm->h; ++j) {
      drawrect(cvs, x+i*sz, y+j*sz, sz, sz, UNPACK_RGBA(color));
    }
  }
}

void draw_box(struct canvas_t* cvs, int x, int y, int w, int h,
               unsigned long outln, unsigned long fill) {
  if (PIXA(fill) != 0) {
    fillrect(cvs, x, y, w, h, UNPACK_RGBA(fill));
  }
  if (PIXA(outln) != 0) {
    drawrect(cvs, x, y, w, h, UNPACK_RGBA(outln));
  }
}

void draw_shape(struct canvas_t* cvs, int x, int y, int sz, int shp,
                unsigned long outln, int alpha) {
/* todo , check if shp > 0 and < *** */

  int i, j, w, h;
  if (shp == -1) return;
  w = g_shps[shp].w;
  h = g_shps[shp].h;
  for (j = 0; j != h; ++j) {
    for (i = 0; i != w; ++i) {
      if (g_shps[shp].pix[j][i]) {
        draw_box(cvs, x+i*sz, y+j*sz, sz, sz, outln,
                  (g_shps[shp].color | PACKA(alpha)));
      }
    }
  }
}
