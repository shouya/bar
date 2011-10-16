#include <dlfcn.h>

#include <bar.h>

#include <ui.h>
#include <export.h>



int main(int argc, char** argv) {
  init_game();

  main_loop();

  destroy_game();

  return 0;
}
