#include "Menu.h"
#include "../utils/Color.h"
#include "Renderer.h"
#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

Menu::Menu(const std::string &title, const std::vector<std::string> &options)
    : title(title), options(options), selectedIndex(0), x(0), y(0), width(40) {}

void Menu::moveUp() {
  selectedIndex--;
  if (selectedIndex < 0) {
    selectedIndex = options.size() - 1; // wrap around al final
  }
}

void Menu::moveDown() {
  selectedIndex++;
  if (selectedIndex >= static_cast<int>(options.size())) {
    selectedIndex = 0; // wrap around al inicio
  }
}

int Menu::getSelectedIndex() const { return selectedIndex; }

std::string Menu::getSelectedOption() const { return options[selectedIndex]; }

void Menu::render() {
  Renderer &renderer = Renderer::getInstance();

  // calcular posicion centrada si no se ha establecido
  if (x == 0 && y == 0) {
    x = (renderer.getWidth() - width) / 2;
    y = (renderer.getHeight() - (options.size() + 6)) / 2;
  }

  // dibujar caja del menu
  renderer.drawBox(x, y, width, options.size() + 4, Color::CYAN);

  // dibujar titulo centrado
  int titleX = x + (width - title.length()) / 2;
  renderer.drawString(titleX, y + 1, title, Color::BRIGHT_YELLOW);

  // linea separadora
  renderer.drawHLine(x + 1, y + 2, width - 2, '-', Color::CYAN);

  // dibujar opciones
  for (size_t i = 0; i < options.size(); i++) {
    int optionY = y + 3 + i;
    std::string prefix = (i == selectedIndex) ? "> " : "  ";
    std::string optionText = prefix + options[i];

    std::string color =
        (i == selectedIndex) ? Color::BRIGHT_GREEN + Color::BOLD : Color::WHITE;

    int optionX = x + (width - optionText.length()) / 2;
    renderer.drawString(optionX, optionY, optionText, color);
  }
}

void Menu::setPosition(int posX, int posY) {
  x = posX;
  y = posY;
}

void Menu::setWidth(int w) { width = w; }
