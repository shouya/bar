/* a simple block game demo, only can drop and clear lines */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <config.h>
#include <shape.h>
#include <blockmap.h>
#include <queue.h>
#include <canvas.h>
#include <ui.h>
#include <defines.h>
#include <graphic.h>

#include <export.h>


struct game_t game;

static struct game_implement {
  struct blockmap_t* bm;
  struct shapebuf_t* sb, *ghost;
  struct queue_t* queue;
  struct canvas_t* cvs, *pnl;
} impl;

static struct control_handler_t demo_ctrl;
static struct render_handler_t demo_render;
static struct event_handler_t demo_event;


static void demo_automove(void);
static void demo_render_render(void);
static void demo_kbd(int k, int mod);

#define BOX_SZ 20
#define XRES 10
#define YRES 20
#define WNDW 300
#define WNDH 400

void init_game(void) {
  srand(time(NULL));

  game.game_name = "Demo";
  game.author_name = "Ya Shou";
  game.author_email = "zxyzxy12321@gmail.com";
  game.impl = &impl;

  demo_ctrl.repeat_on = 1;
  demo_ctrl.repeat_delay = demo_ctrl.repeat_interval = 100;
  demo_ctrl.automove_interval = 300;
  demo_ctrl.auto_move = &demo_automove;
  demo_ctrl.kill_bm_lines = 0;
  demo_ctrl.quit = 0;

  demo_render.render = &demo_render_render;
  demo_render.clrscr = 0;
  demo_render.draw_block = 0;
  demo_render.update = 0;

  demo_event.kbd_handler = &demo_kbd;
  demo_event.quit_handler = 0;

  g_cfg.scrw = WNDW;
  g_cfg.scrh = WNDH;
  g_cfg.ctrl = &demo_ctrl;
  g_cfg.render = &demo_render;
  g_cfg.event = &demo_event;

  init_ui();
  init_graphic();
  
  default_init_shapes();

  impl.bm = create_blockmap(XRES, YRES);
  impl.queue = create_queue(7, 7);
  impl.sb = create_shapebuf(shift_queue(impl.queue));
  impl.sb->x = (impl.bm->w-impl.sb->w)/2;
  impl.ghost = create_shapebuf(impl.sb->shape);
  set_ghost(impl.bm, impl.sb, impl.ghost);
  impl.cvs = create_canvas(BOX_SZ*XRES, BOX_SZ*YRES);
  impl.pnl = create_canvas(WNDW-BOX_SZ*XRES, WNDH);
}


static void demo_automove(void) {
  move_sb(impl.sb, 0, 1);
  if (check_sb(impl.bm, impl.sb) != 0) {
    move_sb(impl.sb, 0, -1);
    if (steady_sb(impl.bm, impl.sb, shift_queue(impl.queue), 3) != 0) {
      printf("game over, you may press C-n to start a new game.\n");
      demo_ctrl.automove_interval = -1;
    }
    set_ghost(impl.bm, impl.sb, impl.ghost);
  }
}

static void demo_render_render(void) {
  clear_canvas(impl.cvs, 0x0);

  draw_bm(impl.cvs, impl.bm, 0, 0, BOX_SZ, 0x7f7f7f7f, 0xff);
  draw_sb(impl.cvs, impl.sb, 0, 0, BOX_SZ, 0xff7f7f7f, 0xe0);
  draw_sb(impl.cvs, impl.ghost, 0, 0, BOX_SZ, 0x0, 0x30);

  draw_grid(impl.cvs, impl.bm, 0, 0, BOX_SZ, 0x10ffffff);

/*  draw_box(impl.cvs);*/

  blit_ui(impl.cvs, 0, 0);
}

static void demo_kbd(int k, int mod) {
  switch (k) {
  case BARK_UP:
    soft_rotate_sb(impl.sb, 1, impl.bm, 2);
    set_ghost(impl.bm, impl.sb, impl.ghost);
    break;
  case BARK_DOWN:
    soft_move_sb(impl.sb, 0, 1, impl.bm);
    break;
  case BARK_LEFT:
    soft_move_sb(impl.sb, -1, 0, impl.bm);
    set_ghost(impl.bm, impl.sb, impl.ghost);
    break;
  case BARK_RIGHT:
    soft_move_sb(impl.sb, 1, 0, impl.bm);
    set_ghost(impl.bm, impl.sb, impl.ghost);
    break;
  case ' ':
    hard_drop_sb(impl.sb, impl.bm);
    break;
  case 'q':
    demo_ctrl.quit = 1;
  default:;
  }
}

void destroy_game(void) {
  destroy_blockmap(impl.bm);
  destroy_canvas(impl.cvs);
  destroy_shapebuf(impl.sb);
  destroy_queue(impl.queue);
  destroy_ui();
}
