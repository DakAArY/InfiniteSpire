#include "Input.h"
#include <bits/types/struct_timeval.h>
#include <cstdio>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

Input *Input::instance = nullptr;

Input::Input() : rawModeEnabled(false) {}

Input::~Input() {
  if (rawModeEnabled) {
    disableRawMode();
  }
}

Input &Input::getInstance() {
  if (instance == nullptr) {
    instance = new Input();
  }
  return *instance;
}

void Input::enableRawMode() {
  if (rawModeEnabled)
    return;

  tcgetattr(STDIN_FILENO, &oldSettings);
  struct termios newSettings = oldSettings;

  newSettings.c_lflag &= ~(ICANON | ECHO);
  newSettings.c_cc[VMIN] = 0;
  newSettings.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
  rawModeEnabled = true;
}

void Input::disableRawMode() {
  if (!rawModeEnabled)
    return;

  tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
  rawModeEnabled = false;
}

bool Input::hasKeyPress() {
  struct timeval tv = {0, 0};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0;
}

Key Input::getKeyPress() {
  if (!hasKeyPress()) {
    return Key::NONE;
  }

  char c = getchar();

  // detectar secuencias de escape (flechas)
  if (c == 27) {
    // dar tiempo para que llegue el resto de la secuencia
    usleep(1000); // 1ms
    if (hasKeyPress()) {
      char next = getchar();
      if (next == '[') {
        usleep(1000);
        if (hasKeyPress()) {
          char arrow = getchar();
          switch (arrow) {
          case 'A':
            return Key::UP;
          case 'B':
            return Key::DOWN;
          case 'C':
            return Key::RIGHT;
          case 'D':
            return Key::LEFT;
          }
        }
      }
    }
    return Key::ESC;
  }

  // Teclas normales
  switch (c) {
  case 'w':
  case 'W':
    return Key::W;
  case 'a':
  case 'A':
    return Key::A;
  case 's':
  case 'S':
    return Key::S;
  case 'd':
  case 'D':
    return Key::D;
  case ' ':
    return Key::SPACE;
  case '\n':
  case '\r':
    return Key::ENTER;
  case 'i':
  case 'I':
    return Key::I;
  case 'm':
  case 'M':
    return Key::M;
  case 'c':
  case 'C':
    return Key::C;
  case 'e':
  case 'E':
    return Key::E;
  case 'q':
  case 'Q':
    return Key::Q;
  case '1':
    return Key::NUM_1;
  case '2':
    return Key::NUM_2;
  case '3':
    return Key::NUM_3;
  case '4':
    return Key::NUM_4;
  case '5':
    return Key::NUM_5;
  default:
    return Key::UNKNOWN;
  }
}

Key Input::waitForKeyPress() {
  Key key = Key::NONE;
  while (key == Key::NONE) {
    key = getKeyPress();
    usleep(10000); // 10ms
  }
  return key;
}
