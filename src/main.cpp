#include "core/Game.h"
#include "utils/Color.h"
#include <exception>
#include <iostream>

int main() {
  try {
    Game game;

    if (!game.initialize()) {
      std::cerr << Color::RED << "Error: no se pudo inicializar el juego"
                << Color::RESET << std::endl;
      return 1;
    }

    game.run();

    return 0;
  } catch (const std::exception &e) {
    std::cerr << Color::RED << "Error fatal: " << e.what() << Color::RESET
              << std::endl;
    return 1;
  }
}
