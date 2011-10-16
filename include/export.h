/* header guard generate by genheader.pl */
#ifndef _export_h_
#define _export_h_

/* game developer should be complete functions and structures below */

struct game_implement;

extern struct game_t {
  const char* game_name;
  const char* author_name;
  const char* author_email;
  struct game_implement* impl;
} game;

extern void init_game(void);

extern void load_default_config(void);


#endif /* _export_h_ */
