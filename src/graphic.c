#include <SDL/SDL.h>

#ifdef ALLOW_IMAGE
#include <SDL/image.h>
#endif /* ALLOW IMAGE */
#ifdef ALLOW_TTF
#include <SDL/ttf.h>
#define FONT_FILENAME "share/fonts/ubuntu-light.ttf"
#endif /* ALLOW TTF */

#include <graphic.h>

struct canvas_t {
  struct SDL_Surface* sfc;
};

#ifdef ALLOW_TTF
TTF_Font* g_font;
#endif /* ALLOW TTF */

void graphic_init(void) {
#ifdef ALLOW_IMAGE
  IMG_Init(IMG_INIT_PNG | IMG_INIT_PNG);
  atexit(&IMG_Quit);
#endif
#ifdef ALLOW_TTF
  TTF_Init();
  atexit(&TTF_Quit);
  g_font = TTF_OpenFont(FONT_FILENAME, FONT_H);
#endif
}


struct canvas_t* create_canvas(int w, int h) {
  struct canvas_t* cvs;
  SDL_Surface* sfc = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32
                                          UNPACK_RGBA(0x00ffffff));
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

void destory_canvas(struct canvas_t* canvas) {
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

void drawpixel(struct canvas_t* canvas, int x, int y, PARAM_RGBA) {
  if (!canvas || !canvas->sfc) return;
  if (x<0 || y<0 || x>=canvas->sfc->clip_rect.w ||
      y>=canvas->sfc->clip_rect.h) return;

  if (a != 0xff) {
    Uint32 o = ((Uint32*)canvas->sfc->pixels)[y*sfc->clip_rect.w+x];
    r = (r*a + PIXR(o)*(0xff-a))>>8;
    g = (g*a + PIXG(o)*(0xff-a))>>8;
    b = (b*a + PIXB(o)*(0xff-a))>>8;
  }
  ((Uint32*)canvas->sfc->pixels)
    [y*canvas->sfc->clip_rect.w+x] = PIXRGB(r,g,b);
}

void drawrect(struct canvas_t* canvas, int x, int y, int w, int h, PARAM_RGBA) {
  int i;
  for (i = x; i != x+w; ++i) {
    drawpixel(canvas->sfc, i, y, r, g, b, a);
    drawpixel(canvas->sfc, i, y+h-1, r, g, b, a);
  }
  for (i = y+1; i != y+h-1; ++i) {
    drawpixel(canvas->sfc, x, i, r, g, b, a);
    drawpixel(canvas->sfc, x+w-1, i, r, g, b, a);
  }
}

void fillrect(struct canvas_t* canvas, int x, int y, int w, int h, PRARM_RGBA) {
  int i, j;
  for (i = x; i != x+w; ++i) {
    for (j = y; j != y+h; ++j) {
      drawpixel(canvas->sfc, i, j, r, g, b, a);
    }
  }
}

/* returns width of the string drawn */
int drawstr(struct canvas_t* canvas, int x, int y, const char* s, PARAM_RGBA) {
#ifdef ALLOW_TTF
  SDL_Surface* txt_sfc;
  int w;
  SDL_Color fg = { r,g,b,0 };
  SDL_Rect dstrect = { x, y, 0, 0 };

  if (!(canvas && canvas->sfc)) return 0;
  if (TTF_SizeUTF8(g_font, s, &w, NULL)) {
    perror(TTF_GetError());
    return 0;
  }
  if (!(txt_sfc = TTF_RenderUTF8_Blended(g_font, s, fg))) {
    perror(TTF_GetError());
    return 0;
  }

  SDL_BlitSurface(txt_sfc, NULL, screen, &dstrect);
  SDL_FreeSurface(txt_sfc);

  return w;
#endif /* ALLOW TTF */
  return 0;
}


