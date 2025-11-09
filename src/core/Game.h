#ifndef GAME_H
#define GAME_H

#include "../utils/Input.h"
#include "../utils/Random.h"
#include "../ui/Renderer.h"
#include "GameState.h"
#include <chrono>

class Game {
private:
  GameState gameState;
  bool running;

  // FPS crontol
  static const int TARGET_FPS = 60;
  static const int FRAME_DELAY = 1000 / TARGET_FPS; // ms pro frame

  std::chrono::steady_clock::time_point lastFrameTime;

  // metodos internos
  void processInput();
  void update(float deltaTime);
  void render();

  // estados especificos
  void handleMainMenu();
  void handleTower();
  void handleHub();
  void handleCombat();
  void handlePause();

public:
  Game();
  ~Game();

  // inicialñizar juego
  bool initialize();

  // loop principal
  void run();

  // limpiar recurtsos
  void cleanup();

  // control de juego
  void quit();
  bool isRunning() const;
};

#endif
