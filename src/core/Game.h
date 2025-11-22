#ifndef GAME_H
#define GAME_H

#include "../ui/Menu.h"
#include "../ui/Renderer.h"
#include "../ui/UIManager.h"
#include "../utils/Input.h"
#include "../utils/Random.h"
#include "GameState.h"
#include <chrono>
#include <memory>

class Game {
private:
  GameState gameState;
  bool running;

  // FPS crontol
  static const int TARGET_FPS = 60;
  static const int FRAME_DELAY = 1000 / TARGET_FPS; // ms pro frame

  std::chrono::steady_clock::time_point lastFrameTime;

  // Menu principal
  std::unique_ptr<Menu> mainMenu;
  std::unique_ptr<Menu> pauseMenu;

  // UI Manager
  std::unique_ptr<UIManager> uiManager;

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
