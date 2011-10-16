#include <config.h>
#include <shape.h>

#include <export.h>



struct game_t {
  const char* game_name;
  const char* author_name;
  const char* author_email;
} game;

void init_game(void) {
  game_name = "Demo";
  author_name = "Ya Shou";
  author_email = "zxyzxy12321@gmail.com";
}

void load_default_config(void) {
  g_cfg.name = game_name;
  g_cfg.scrw = 300;
  g_cfg.scrh = 400;
}


void init_shapes(void) {
}

