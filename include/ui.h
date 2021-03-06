/* header guard generate by genheader.pl */
#ifndef _ui_h_
#define _ui_h_

struct SDL_Surface;
struct control_handler_t;
struct render_handler_t;

struct ui_t {
  struct SDL_Surface* scr;
};


extern struct ui_t* g_ui;


int init_ui(void);
void destroy_ui(void);

void main_loop(void);


#endif /* _ui_h_ */
