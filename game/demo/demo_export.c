/* a simple block game demo, only can drop and clear lines */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <canvas.h>
#include <config.h>
#include <shape.h>
#include <blockmap.h>
#include <queue.h>
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
  int holdbuf, canhold;
} impl;

static struct control_handler_t demo_ctrl;
static struct render_handler_t demo_render;
static struct event_handler_t demo_event;

static void demo_automove(void);
static void demo_render_render(void);
static void demo_kbd(int k, int mod);
static void demo_newgame(void);
static int demo_getnext(void);

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
  impl.sb = create_shapebuf(-1);
  impl.ghost = create_shapebuf(impl.sb->shape);
  impl.cvs = create_canvas(BOX_SZ*XRES, BOX_SZ*YRES);
  impl.pnl = create_canvas(WNDW-BOX_SZ*XRES, WNDH);

  demo_newgame();
}


static void demo_automove(void) {
  move_sb(impl.sb, 0, 1);
  if (check_sb(impl.bm, impl.sb) != 0) {
    move_sb(impl.sb, 0, -1);
    if (steady_sb(impl.bm, impl.sb, shift_queue(impl.queue), 3) != 0) {
      printf("game over, you may press C-n to start a new game.\n");
      demo_ctrl.automove_interval = -1;
      clear_blockmap(impl.bm);
      reset_shapebuf(impl.sb, -1);
    }
    set_ghost(impl.bm, impl.sb, impl.ghost);
    impl.canhold = 1;
  }
}

static void demo_render_render(void) {
  clear_canvas(impl.cvs, 0x0);
  clear_canvas(impl.pnl, 0x0);

  draw_bm(impl.cvs, impl.bm, 0, 0, BOX_SZ, 0x7f7f7f7f, 0xff);
  draw_sb(impl.cvs, impl.sb, 0, 0, BOX_SZ, 0xff7f7f7f, 0xe0);
  draw_sb(impl.cvs, impl.ghost, 0, 0, BOX_SZ, 0x0, 0x30);

  draw_grid(impl.cvs, impl.bm, 0, 0, BOX_SZ, 0x10ffffff);

  draw_box(impl.pnl, 0, 0, 100, 100, 0x0, 0xff111111);
  draw_shape_center(impl.pnl, 0, 0, 100, 100, 25, impl.holdbuf, 0x0, 0xff);

  draw_shape_center(impl.pnl, 0, 120, 50, 50, 12,
                    impl.queue->queue[0], 0x3f<<24, 0xf0);
  draw_shape_center(impl.pnl, 55, 120, 40, 40, 10,
                    impl.queue->queue[1], 0x2f<<24, 0xc0);
  draw_shape_center(impl.pnl, 55, 160, 40, 40, 10,
                    impl.queue->queue[2], 0x1f<<24, 0x80);
  draw_shape_center(impl.pnl, 55, 200, 40, 40, 10,
                    impl.queue->queue[3], 0x0f<<24, 0x50);

  blit_ui(impl.cvs, 0, 0);
  blit_ui(impl.pnl, BOX_SZ*XRES, 0);
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
    break;
  case 'c': /* hold */
    if (impl.canhold) {
      swap_hold(&impl.holdbuf, impl.sb, &demo_getnext,
                impl.bm, 3);
      set_ghost(impl.bm, impl.sb, impl.ghost);
      impl.canhold = 0;
    }
    break;
  case 'n':
    if (mod & BARKM_CTRL) {
      demo_newgame();
    }
  default:;
  }
}

void destroy_game(void) {
  destroy_blockmap(impl.bm);
  destroy_canvas(impl.cvs);
  destroy_canvas(impl.pnl);
  destroy_shapebuf(impl.sb);
  destroy_shapebuf(impl.ghost);
  destroy_queue(impl.queue);
  destroy_ui();
}

static void demo_newgame(void) {
  impl.holdbuf = -1;
  impl.canhold = 1;
  demo_ctrl.automove_interval = 300;
  clear_blockmap(impl.bm);
  soft_reset_sb(impl.sb, shift_queue(impl.queue), impl.bm, 3);
  set_ghost(impl.bm, impl.sb, impl.ghost);
}

static int demo_getnext(void) {
  return shift_queue(impl.queue);
}
