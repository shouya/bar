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
  } else {
    fillrect(cvs, x, y, sz, sz, UNPACK_RGB(g_shps[blk->shape].color), alpha);
    drawrect(cvs, x, y, sz, sz, UNPACK_RGBA(outline));
  }
}
void draw_sb(struct canvas_t* cvs, const struct shapebuf_t* sb,
             int x, int y, int sz, unsigned long outline, int alpha) {
  int i, j;
  struct block_t blk = {1, sb->shape, 0};
  for (j = 0; j != sb->h; ++j) {
    for (i = 0; i != sb->w; ++i) {
      if (sb->buf[j*sb->w+i]) {
        draw_block(cvs, &blk, x + i*sz, y + j*sz, sz, outline, alpha);
      }
    }
  }
}

void draw_bm(struct canvas_t* cvs, const struct blockmap_t* sb,
             int x, int y, int sz, unsigned long outline, int alpha) {
  int i, j;
  for (i = 0; i != sb->w; ++i) {
    for (j = 0; j != sb->h; ++j) {
      if (sb->buf[j*sb->w+i].occupied) {
        draw_block(cvs, &(sb->buf[j*sb->w+i]),
                   x+i*sz, y+i*sz, sz, outline, alpha);
      }
    }
  }
}
