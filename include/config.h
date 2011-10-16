/* header guard generate by genheader.pl */
#ifndef _config_h_
#define _config_h_

struct block_t;
struct canvas_t;

struct render_handler_t {
  void (*render)(void); /* essential */
  void (*clrscr)(void); /* optional */
  void (*draw_block)(struct canvas_t* cvs, const struct block_t* blk,
                     int x, int y, int sz,
                     unsigned long lncolor, int alpha); /* optional */
  void (*update)(void); /* optional */
};

struct control_handler_t {
  int repeat_on, repeat_delay, repeat_interval;
  int automove_interval; /* essential */
  void (*automove)(void); /* essential */
};

struct event_handler_t {
  void (*kbd_handler)(int key, int mod); /* essential */
  void (*quit_handler)(int* quit); /* optional */
};


struct config_t {
  int scrw, scrh;
  struct control_handler_t* ctrl;
  struct event_handler_t* event;
  struct render_handler_t* render;
  int break_main_mainloop;
};



extern struct config_t g_cfg;

void load_config(const char* filename);


#endif /* _config_h_ */
