#include "gui/cli/frontend.h"

int main(void) {
  if (!init_frontend()) {
    fprintf(stderr, "Failed to initialize frontend\n");
    return 1;
  }

  while (1) {
    if (!show_game_selection()) {
      break;
    }

    init_selected_game();
    run_game_loop();
    shutdown_current_game();
  }
  cleanup_frontend();

  return 0;
}
