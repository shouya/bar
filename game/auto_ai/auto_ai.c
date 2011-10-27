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

#include <util.h>
#include <ai.h>

#include <export.h>


struct game_t game;

static struct game_implement {
  struct blockmap_t* bm;
  struct shapebuf_t* sb/*, *ghost*/;
  struct queue_t* queue;
  struct canvas_t* cvs, *pnl;
  struct ai_t* ai;
} impl;

static unsigned long beg_time;
static int lines_killed;

static struct control_handler_t game_ctrl;
static struct render_handler_t game_render;
static struct event_handler_t game_event;

static void do_auto_move(void);
static void do_step_ai(void);
static void do_render(void);
static void kill_lines_hook(struct blockmap_t* bm, int* lnbuf, int num);

static void dealkbd(int k, int mod);
static void newgame(void);

#define BOX_SZ 20
#define XRES 10
#define YRES 20
#define WNDW 300
#define WNDH 400

void init_game(void) {
  srand(time(NULL));

  game.game_name = "Auto AI";
  game.author_name = "Ya Shou";
  game.author_email = "zxyzxy12321@gmail.com";
  game.impl = &impl;

  game_ctrl.repeat_on = 0;
  game_ctrl.automove_interval = 300;
  game_ctrl.auto_move = &do_auto_move;
  game_ctrl.before_auto_move = &do_step_ai;
  game_ctrl.kill_bm_lines = &kill_lines_hook;
  game_ctrl.quit = 0;

  game_render.render = &do_render;
  game_render.clrscr = 0;
  game_render.draw_block = 0;
  game_render.update = 0;

  game_event.kbd_handler = &dealkbd;
  game_event.quit_handler = 0;

  g_cfg.scrw = WNDW;
  g_cfg.scrh = WNDH;
  g_cfg.ctrl = &game_ctrl;
  g_cfg.render = &game_render;
  g_cfg.event = &game_event;

  init_ui();
  init_graphic();
  
  default_init_shapes();

  impl.bm = create_blockmap(XRES, YRES);
  impl.queue = create_queue(7, 7);
  impl.sb = create_shapebuf(-1);
  impl.cvs = create_canvas(BOX_SZ*XRES, BOX_SZ*YRES);
  impl.pnl = create_canvas(WNDW-BOX_SZ*XRES, WNDH);

  newgame();
}



static void do_auto_move(void) {
  move_sb(impl.sb, 0, 1);
  if (check_sb(impl.bm, impl.sb) != 0) {
    move_sb(impl.sb, 0, -1);
    lines_killed += check_bm_lines(impl.bm, 0, 0);
    if (steady_sb(impl.bm, impl.sb, shift_queue(impl.queue), 3) != 0) {
      /*printf("game over, you may press C-n to start a new game.\n");*/
      printf("this AI cleared lines: %d, spend: %.2fs\n", lines_killed,
             (util_getticks()-beg_time)/1000.0f);
      game_ctrl.automove_interval = -1;
      clear_blockmap(impl.bm);
      reset_shapebuf(impl.sb, -1);
      newgame();
    } else {
      impl.ai = ai_calc(impl.bm, impl.sb, 0, 0);
    }
  }
}

static void do_step_ai(void) {
  static unsigned long tick = 0;
  if (util_getticks() - tick >= 1) {
    ai_step(impl.ai, impl.bm, impl.sb);
    tick = util_getticks();
  }
}


static void do_render(void) {
  clear_canvas(impl.cvs, 0x0);
  clear_canvas(impl.pnl, 0x0);

  draw_bm(impl.cvs, impl.bm, 0, 0, BOX_SZ, 0x7f7f7f7f, 0xff);
  draw_sb(impl.cvs, impl.sb, 0, 0, BOX_SZ, 0xff7f7f7f, 0xe0);

  draw_grid(impl.cvs, impl.bm, 0, 0, BOX_SZ, 0x10ffffff);

  draw_box(impl.pnl, 0, 0, 100, 100, 0x0, 0xff111111);

  draw_shape_center(impl.pnl, 0, 20, 50, 50, 12,
                    impl.queue->queue[0], 0x3f<<24, 0xf0);
  draw_shape_center(impl.pnl, 55, 20, 40, 40, 10,
                    impl.queue->queue[1], 0x2f<<24, 0xc0);
  draw_shape_center(impl.pnl, 55, 60, 40, 40, 10,
                    impl.queue->queue[2], 0x1f<<24, 0x80);
  draw_shape_center(impl.pnl, 55, 100, 40, 40, 10,
                    impl.queue->queue[3], 0x0f<<24, 0x50);

  blit_ui(impl.cvs, 0, 0);
  blit_ui(impl.pnl, BOX_SZ*XRES, 0);
}

static void dealkbd(int k, int mod) {
  switch (k) {
  case 'q':
    game_ctrl.quit = 1;
    break;
  case 'n':
    if (mod & BARKM_CTRL) {
      newgame();
    }
  default:;
  }
}

void destroy_game(void) {
  destroy_blockmap(impl.bm);
  destroy_canvas(impl.cvs);
  destroy_canvas(impl.pnl);
  destroy_shapebuf(impl.sb);
  destroy_queue(impl.queue);
  destroy_ui();
}

static void newgame(void) {
  game_ctrl.automove_interval = 300;
  clear_blockmap(impl.bm);
  soft_reset_sb(impl.sb, shift_queue(impl.queue), impl.bm, 3);
  impl.ai = ai_calc(impl.bm, impl.sb, 0, 0);
  lines_killed = 0;
  beg_time = util_getticks();
}

static void kill_lines_hook(struct blockmap_t* bm, int* lnbuf, int num) {
  game_ctrl.kill_bm_lines = 0;
  lines_killed += num;
  kill_bm_lines(bm, lnbuf, num);
  game_ctrl.kill_bm_lines = &kill_lines_hook;
}
