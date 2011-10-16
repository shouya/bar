#include <stdlib.h>

#include <SDL/SDL.h>

#include <ui.h>
#include <config.h>

ui_t* g_ui = 0;

int init_ui(int w, int h) {
  if (g_ui) {
    return -1;
  }

  g_ui = calloc(1, sizeof(struct ui_t));
  SDL_Init(SDL_INIT_EVERYTHING);
  g_ui->scr = SDL_SetVideoMode(g_cfg->scrw, g_cfg->scrh, 32,
                               SDL_SWSURFACE | SDL_DOUBLEBUF);
  if (!g_ui->src) {
    perror(SDL_GetError());
    return -1;
  }

  SDL_WM_SetCaption(g_cfg->name);

  return 0;
}

void destroy_ui(void) {
  if (g_ui) {
    if (g_ui->src) {
      SDL_FreeSurface(g_ui->scr);
    }
    free(g_ui);
  }
}


void mainloop(void) {
  if (g_cfg.render) {
    (*g_cfg->render->clrscr)();
  }
}
