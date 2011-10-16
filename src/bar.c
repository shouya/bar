#include <bar.h>

#include <ui.h>
#include <export.h>

int main(int argc, char** argv) {
  init_game();
  init_ui();

  main_loop();

  destroy_ui();
  destroy_game();
}
