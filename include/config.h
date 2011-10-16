/* header guard generate by genhead.pl */
#ifndef _config_h_
#define _config_h_




struct config_t {
  char* name;
  int scrw, scrh;
  struct control_handler_t* ctrl;
  struct render_handler_t* render;
  int break_main_mainloop;
};

extern struct config_t g_cfg;

void load_config(const char* filename);


#endif /* _config_h_ */
