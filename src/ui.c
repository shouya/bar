#include <stdlib.h>

#include <SDL/SDL.h>

#include <ui.h>
#include <config.h>

ui_t* g_ui = 0;

static void clrscr(void);
static void update(void);
static void quit_handler(int* quit);
static void automove(void);

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

  if (g_cfg->ctrl->repeat_on != 0) {
    SDL_EnableKeyRepeat(g_cfg->ctrl->repeat_delay,
                        g_cfg->ctrl->repeat_interval);
  }

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
  SDL_Event event;
  int quit = 0;
  unsigned long tick = 0;

  while (!quit) {
    if (SDL_PoolEvent(event)) {
      switch (event.type) {
      case SDL_QUIT:
        if (g_cfg->event->quit_handler) {
          (*g_cfg->event->quit_handler )(&quit);
        } else {
          quit_handler(&quit);
        }
        break;
      case SDL_KEYDOWN:
        if (g_cfg->event->kbd_handler) {
          (*g_cfg->event->kbd_handler)(event.key.keysym.sym,
                                       event.key.keysym.mod);
        }
        break;
      default:;
      }
    }

    if (SDL_GetTicks() - tick > g_cfg->ctrl->automove_interval) {
      (*g_cfg->ctrl->automove)();
    }

    if (g_cfg->render->clrscr) {
      (*g_cfg->render->clrscr)();
    } else {
      clrscr();
    }

    (*g_cfg.render->render)();

    if (g_cfg->render->update) {
      (*g_cfg->render->update)();
    } else {
      update();
    }
  } /* while no quit */
}

void clrscr(void) {
  SDL_FillRect(g_ui->scr, 0, 0);
}

void update(void) {
  SDL_Flip(g_ui->scr);
}

void quit_handler(int* quit) {
  *quit = 1;
}



