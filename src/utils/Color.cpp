#include "Color.h"
#include "Random.h"
#include <iostream>
#include <string>

namespace Color {
std::string colorize(const std::string &text, const std::string &color) {
  return color + text + RESET;
}

std::string apply(const std::string &text, const std::string &color,
                  const std::string &style) {
  if (style.empty()) {
    return color + text + RESET;
  }
  return style + color + text + RESET;
}

std::string getRandomWallColor() {
  Random &rng = Random::getInstance();
  int choice = rng.getInt(0, 7);

  switch (choice) {
  case 0:
    return BRIGHT_RED;
  case 1:
    return BRIGHT_GREEN;
  case 2:
    return BRIGHT_YELLOW;
  case 3:
    return BRIGHT_BLUE;
  case 4:
    return BRIGHT_MAGENTA;
  case 5:
    return BRIGHT_CYAN;
  case 6:
    return CYAN;
  case 7:
    return YELLOW;
  default:
    return WHITE;
  }
}

std::string getRarityColor(int rarity) {
  switch (rarity) {
  case 0:
    return COMMON;
  case 1:
    return RARE;
  case 2:
    return EPIC;
  case 3:
    return LEGENDARY;
  default:
    return WHITE;
  }
}

void clearScreen() { std::cout << "\033[2J\033[1;1H"; }

void moveCursor(int x, int y) { std::cout << "\033[" << y << ";" << x << "H"; }

void hideCursor() { std::cout << "\033[?25l"; }

void showCursor() { std::cout << "\033[?25h"; }
} // namespace Color
