#include "Renderer.h"
#include "../utils/Color.h"
#include <asm-generic/ioctls.h>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

Renderer *Renderer::instance = nullptr;

Renderer::Renderer() : width(80), height(24) {}

Renderer &Renderer::getInstance() {
  if (instance == nullptr) {
    instance = new Renderer();
  }
  return *instance;
}

void Renderer::initialize() {
  // obtener tamaño de terminal
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  width = w.ws_col;
  height = w.ws_row;

  initBuffers();
  hideCursor();
  clear();
}

void Renderer::initBuffers() {
  buffer.resize(height, std::vector<char>(width, ' '));
  colorBuffer.resize(height, std::vector<std::string>(width, ""));
}

void Renderer::clearBuffers() {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      buffer[y][x] = ' ';
      colorBuffer[y][x] = "";
    }
  }
}

void Renderer::clear() {
  std::cout << "\033[2J\033[H" << std::flush;
  clearBuffers();
}

void Renderer::drawChar(int x, int y, char c, const std::string &color) {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    buffer[y][x] = c;
    colorBuffer[y][x] = color;
  }
}

void Renderer::drawString(int x, int y, const std::string &str,
                          const std::string &color) {
  int pos = x;
  for (char c : str) {
    if (pos >= width)
      break;
    drawChar(pos, y, c, color);
    pos++;
  }
}

void Renderer::drawHLine(int x, int y, int lenght, char c,
                         const std::string &color) {
  for (int i = 0; i < lenght; i++) {
    drawChar(x + i, y, c, color);
  }
}

void Renderer::drawVLine(int x, int y, int lenght, char c,
                         const std::string &color) {
  for (int i = 0; i < lenght; i++) {
    drawChar(x, y + i, c, color);
  }
}

void Renderer::drawBox(int x, int y, int w, int h, const std::string &color) {
  // equinas
  drawChar(x, y, '+', color);
  drawChar(x + w - 1, y, '+', color);
  drawChar(x, y + h - 1, '+', color);
  drawChar(x + w - 1, y + h - 1, '+', color);

  // lineas horizontales
  drawHLine(x + 1, y, w - 2, '-', color);
  drawHLine(x + 1, y + h - 1, w - 2, '-', color);

  // lineas verticales
  drawVLine(x, y + 1, h - 2, '|', color);
  drawVLine(x + w - 1, y + 1, h - 2, '|', color);
}

void Renderer::drawCentered(int y, const std::string &str,
                            const std::string &color) {
  int x = (width - str.length()) / 2;
  if (x < 0)
    x = 0;
  drawString(x, y, str, color);
}

void Renderer::present() {
  moveCursor(0, 0);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (!colorBuffer[y][x].empty()) {
        std::cout << colorBuffer[y][x];
      }
      std::cout << buffer[y][x];
      if (!colorBuffer[y][x].empty()) {
        std::cout << Color::RESET;
      }
    }
    if (y < height - 1) {
      std::cout << '\n';
    }
  }
  std::cout << std::flush;
}

int Renderer::getWidth() const { return width; }

int Renderer::getHeight() const { return height; }

void Renderer::hideCursor() { std::cout << "\033[?251" << std::flush; }

void Renderer::showCursor() { std::cout << "\033[?25h" << std::flush; }

void Renderer::moveCursor(int x, int y) {
  std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
}
