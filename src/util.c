#include <SDL/SDL.h>

#include <util.h>

void util_delay(unsigned long ms) {
  SDL_Delay(ms);
}

unsigned long util_getticks(void) {
  return SDL_GetTicks();
}

