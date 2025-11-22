#ifndef INPUT_H
#define INPUT_H

#include <experimental/filesystem>
#include <termios.h>
#include <unistd.h>

enum class Key {
  NONE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  W,
  A,
  S,
  D,
  SPACE,
  ENTER,
  ESC,
  BACKSPACE,
  TILDE,
  I, // inventario
  M, // mapa
  C, // character
  E, // usar o interactuar
  Q, // opcion de quit
  NUM_1,
  NUM_2,
  NUM_3,
  NUM_4,
  NUM_5,
  CHAR_START = 32,  // espacio
  CHAR_END = 126,   // ~
  UNKNOWN
};

class Input {
private:
  struct termios oldSettings;
  bool rawModeEnabled;

  static Input *instance;
  Input();

public:
  ~Input();

  static Input &getInstance();

  // Activar o desactivar el modo raw (sin esperar ENTER)
  void enableRawMode();
  void disableRawMode();

  // leer tecla sin bloquear (non-blocking)
  Key getKeyPress();

  // leer tecla bloqueando hasta que se presione
  Key waitForKeyPress();

  // verificar si hay tecla disponible+
  bool hasKeyPress();
};

#endif
