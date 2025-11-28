#include "gui/cli/frontend.h"

int main(void) {
  if (!init_frontend()) {
    fprintf(stderr, "Failed to initialize frontend\n");
    return 1;
  }

  init_game();
  run_game_loop();
  cleanup_game();
  cleanup_frontend();

  return 0;
}