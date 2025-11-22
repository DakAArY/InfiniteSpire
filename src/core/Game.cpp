#include "Game.h"
#include "../utils/Color.h"
#include "GameState.h"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>
#include <thread>
#include <vector>

Game::Game() : running(false) {
  // Crear menu principal
  std::vector<std::string> options = {"Nuevo Juego", "Cargar Partida", "Salir"};
  mainMenu = std::make_unique<Menu>("THE INFINITE SPIRE", options);

  // crear menu de pausa
  std::vector<std::string> pauseOptions = {"Reanudar",
                                           "Salir al Menu principal"};
  pauseMenu = std::make_unique<Menu>("PAUSA", pauseOptions);

  // crear UI Manager
  uiManager = std::make_unique<UIManager>();
}

Game::~Game() { cleanup(); }

bool Game::initialize() {
  // inicializar sistema de random con semilla de fecha
  Random::getInstance().setSeedFromDate();

  // inicializar renderer
  Renderer::getInstance().initialize();

  // inicializar input en modo raw+
  Input::getInstance().enableRawMode();

  running = true;
  lastFrameTime = std::chrono::steady_clock::now();

  return true;
}

void Game::run() {
  while (running) {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       currentTime - lastFrameTime)
                       .count();

    float deltaTime = elapsed / 1000.0f;
    lastFrameTime = currentTime;

    processInput();
    update(deltaTime);
    render();

    // control de FPS
    auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::steady_clock::now() - currentTime)
                         .count();

    if (frameTime < FRAME_DELAY) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(FRAME_DELAY - frameTime));
    }
  }
}

void Game::processInput() {
  Key key = Input::getInstance().getKeyPress();
  State current = gameState.getState();

  // Toggle consola de debug con tecla ~
  if (key == Key::TILDE && current != State::MAIN_MENU) {
    uiManager->toggleConsole();
    return;
  }

  // Si la consola está activa, manejar input de consola
  if (uiManager->isConsoleActive()) {
    if (key == Key::ENTER) {
      uiManager->submitConsoleCommand();
    } else if (key == Key::BACKSPACE) {
      uiManager->removeConsoleInput();
    } else if (key == Key::UP) {
      uiManager->navigateHistory(true);
    } else if (key == Key::DOWN) {
      uiManager->navigateHistory(false);
    } else if (key == Key::ESC) {
      uiManager->toggleConsole();
    } else if (key >= Key::CHAR_START && key <= Key::CHAR_END) {
      uiManager->addConsoleInput((char)key);
    }
    return;
  }

  // Input para menu principal
  if (current == State::MAIN_MENU) {
    if (key == Key::UP || key == Key::W) {
      mainMenu->moveUp();
    } else if (key == Key::DOWN || key == Key::S) {
      mainMenu->moveDown();
    } else if (key == Key::ENTER || key == Key::SPACE) {
      int selected = mainMenu->getSelectedIndex();
      if (selected == 0) { // Nuevo Juego
        gameState.setState(State::TOWER);
      } else if (selected == 1) { // Cargar Partida
        // TODO: implementar carga
        gameState.setState(State::TOWER);
      } else if (selected == 2) { // Salir
        quit();
      }
    }
    return;
  }

  // Input para menu de pausa
  if (current == State::PAUSE) {
    if (key == Key::UP || key == Key::W) {
      pauseMenu->moveUp();
    } else if (key == Key::DOWN || key == Key::S) {
      pauseMenu->moveDown();
    } else if (key == Key::ENTER || key == Key::SPACE) {
      int selected = pauseMenu->getSelectedIndex();
      if (selected == 0) { // reanudar
        gameState.returnToPreviousState();
      } else if (selected == 1) { // Salir al menu principal
        gameState.setState(State::MAIN_MENU);
      }
    }
    return;
  }

  // Input general para otros estados
  if (key == Key::ESC && current != State::MAIN_MENU) {
    if (current == State::PAUSE) {
      gameState.returnToPreviousState();
    } else {
      gameState.setState(State::PAUSE);
    }
  }
}

void Game::update(float deltaTime) {
  State current = gameState.getState();

  switch (current) {
  case State::MAIN_MENU:
    handleMainMenu();
    break;
  case State::TOWER:
    handleTower();
    break;
  case State::HUB:
    handleHub();
    break;
  case State::COMBAT:
    handleCombat();
    break;
  case State::PAUSE:
    handlePause();
    break;
  case State::EXIT:
    quit();
    break;
  default:
    break;
  }
}

void Game::render() {
  Renderer &renderer = Renderer::getInstance();
  renderer.clear();

  State current = gameState.getState();

  // Renderizar según estado
  switch (current) {
  case State::MAIN_MENU:
    mainMenu->render();
    renderer.drawString(2, renderer.getHeight() - 2,
                        "Controles: W/S=Navegar ENTER/SPACE=Seleccionar",
                        Color::BRIGHT_BLACK);
    break;

  case State::PAUSE: {
    // renderizar estado anterior de fondo
    State previous = gameState.getPreviousState();
    if (previous == State::TOWER) {
      int centerY = renderer.getHeight() / 2;
      int boxWidth = 50;
      int boxHeight = 10;
      int boxX = (renderer.getWidth() - boxWidth) / 2;
      int boxY = centerY - boxHeight / 2;

      renderer.drawBox(boxX, boxY, boxWidth, boxHeight,
                       Color::GREEN + Color::DIM);
      renderer.drawCentered(boxY + 2, "LA TORRE", Color::GREEN + Color::DIM);
      renderer.drawCentered(boxY + 5, "Piso: 1", Color::WHITE + Color::DIM);
    }

    // renderizar menu de pausa encima
    pauseMenu->render();
    renderer.drawString(2, renderer.getHeight() - 2,
                        "Controles: W/S=Navegar ENTER/SPACE=Seleccionar",
                        Color::BRIGHT_BLACK);
    break;
  }

  case State::TOWER: {
    // Renderizar HUD
    uiManager->renderHUD();

    int centerY = renderer.getHeight() / 2;
    int boxWidth = 50;
    int boxHeight = 10;
    int boxX = (renderer.getWidth() - boxWidth) / 2;
    int boxY = centerY - boxHeight / 2;

    renderer.drawBox(boxX, boxY, boxWidth, boxHeight, Color::GREEN);
    renderer.drawCentered(boxY + 2, "LA TORRE", Color::GREEN + Color::BOLD);
    renderer.drawCentered(boxY + 5, "Piso: 1", Color::WHITE);
    renderer.drawCentered(boxY + 7, "[ESC] Pausa | [~] Debug Console", Color::WHITE);

    // Renderizar consola si está activa
    uiManager->renderConsole();
    break;
  }

  case State::HUB: {
    // Renderizar HUD
    uiManager->renderHUD();

    int centerY = renderer.getHeight() / 2;
    int boxWidth = 50;
    int boxHeight = 10;
    int boxX = (renderer.getWidth() - boxWidth) / 2;
    int boxY = centerY - boxHeight / 2;

    renderer.drawBox(boxX, boxY, boxWidth, boxHeight, Color::MAGENTA);
    renderer.drawCentered(boxY + 2, "HUB - CAMPAMENTO",
                          Color::MAGENTA + Color::BOLD);
    renderer.drawCentered(boxY + 5, "Area segura", Color::WHITE);
    renderer.drawCentered(boxY + 7, "[ESC] Pausa | [~] Debug Console", Color::WHITE);

    // Renderizar consola si está activa
    uiManager->renderConsole();
    break;
  }

  default:
    break;
  }

  renderer.present();
}

void Game::handleMainMenu() {
  // El menu se maneja en processInput y render
}

void Game::handleTower() {
  // Simular cambios de stats para pruebas (puedes comentar esto después)
  static float testTime = 0.0f;
  testTime += 0.001f;
  if (testTime > 1.0f) testTime = 0.0f;
  uiManager->setDayNightTime(testTime);
}

void Game::handleHub() {
  // ToDo implementar logica de hub
}

void Game::handleCombat() {
  // ToDo implementar logica de combate
}

void Game::handlePause() {
  // ToDo implementar menu de pausa
}

void Game::cleanup() {
  Renderer::getInstance().showCursor();
  Input::getInstance().disableRawMode();
  system("clear");
  std::cout << Color::BRIGHT_MAGENTA << "Gracias por jugar" << Color::RESET
            << std::endl;
}

void Game::quit() { running = false; }

bool Game::isRunning() const { return running; }
