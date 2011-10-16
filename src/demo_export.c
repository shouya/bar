/* a simple block game demo, only can drop and clear lines */

#include <config.h>
#include <shape.h>
#include <blockmap.h>
#include <queue.h>
#include <canvas.h>

#include <export.h>


struct game_t game;

static struct game_implement {
  struct blockmap_t* bm;
  struct shapebuf_t* sb;
  struct queue_t* queue;
  struct canvas_t* cvs;
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

void init_game(void) {
  game.game_name = "Demo";
  game.author_name = "Ya Shou";
  game.author_email = "zxyzxy12321@gmail.com";
  game.impl = &impl;
  
  default_init_shapes();

  impl.bm = create_blockmap(XRES, YRES);
  impl.queue = create_queue(7, 7);
  impl.sb = create_shapebuf(shift_queue(impl.queue));
  impl.cvs = create_canvas(BOX_SZ*XRES, BOX_SZ*YRES);

  demo_ctrl.repeat_on = 1;
  demo_ctrl.repeat_delay = demo_ctrl.repeat_interval = 100;
  demo_ctrl.automove_interval = 300;
  demo_ctrl.automove = &demo_automove;

  demo_render.render = &demo_render_render;
  demo_render.clrscr = 0;
  demo_render.draw_block = 0;
  demo_render.update = 0;

  demo_event.kbd_handler = &demo_kbd;
  demo_event.quit_handler = 0;

  g_cfg.scrw = 300;
  g_cfg.scrh = 400;
  g_cfg.ctrl = &demo_ctrl;
  g_cfg.render = &demo_render;
  g_cfg.event = &demo_event;
}


static void demo_automove(void) {
  move_sb(impl.sb, 0, 1);
  if (check_sb(impl.bm, impl.sb) != 0) {
    move_sb(impl.sb, 0, -1);
    merge_sb(impl.bm, impl.sb);
  }
}

static void demo_render_render(void) {
  draw_bm(impl.cvs, impl.bm, 0, 0, BOX_SZ, 0x7f7f7f7f, 0xff);
  draw_sb(impl.cvs, impl.sb, 0, 0, BOX_SZ, 0xff7f7f7f, 0xe0);
}

static void demo_kbd(int k, int mod) {
  switch (k) {
  case 'w':
    rotate_sb(impl.sb, 1);
    if (check_sb(impl.bm, impl.sb) != 0) {
      rotate_sb(impl.sb, -1);
    }
    break;
  case 's':
    move_sb(impl.sb, 0, 1);
    if (check_sb(impl.bm, impl.sb) != 0) {
      move_sb(impl.sb, 0, -1);
    }
    break;
  case 'a':
    move_sb(impl.sb, -1, 0);
    if (check_sb(impl.bm, impl.sb) != 0) {
      move_sb(impl.sb, 1, 0);
    }
    break;
  case 'd':
    move_sb(impl.sb, 1, 0);
    if (check_sb(impl.bm, impl.sb) != 0) {
      move_sb(impl.sb, -1, 0);
    }
    break;
  case ' ':
    while (check_sb(impl.bm, impl.sb) == 0) {
      move_sb(impl.sb, 0, 1);
    }
    move_sb(impl.sb, 0, -1);

    break;
  default:;
  }
}

void destroy_game(void) {
  destroy_blockmap(impl.bm);
  destroy_canvas(impl.cvs);
  destroy_shapebuf(impl.sb);
  destroy_queue(impl.queue);
  
}
