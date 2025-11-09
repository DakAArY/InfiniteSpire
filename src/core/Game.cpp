#include "Game.h"
#include "../utils/Color.h"
#include "GameState.h"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <thread>

Game::Game() : running(false) {}

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

  if (key == Key::ESC && gameState.getState() != State::MAIN_MENU) {
    if (gameState.getState() == State::PAUSE) {
      gameState.returnToPreviousState();
    } else {
      gameState.setState(State::PAUSE);
    }
  }

  if (key == Key::Q && gameState.getState() == State::MAIN_MENU) {
    quit();
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
  Renderer& renderer = Renderer::getInstance();
  State current = gameState.getState();
  
  int centerY = renderer.getHeight() / 2;
  int boxWidth = 50;
  int boxHeight = 10;
  int boxX = (renderer.getWidth() - boxWidth) / 2;
  int boxY = centerY - boxHeight / 2;
  
  // Dibujar caja principal
  renderer.drawBox(boxX, boxY, boxWidth, boxHeight, Color::BRIGHT_CYAN);
  
  // Título centrado
  renderer.drawCentered(boxY + 2, "THE INFINITE SPIRE", Color::BRIGHT_CYAN);
  
  // Línea separadora
  renderer.drawHLine(boxX + 2, boxY + 3, boxWidth - 4, '=', Color::CYAN);
  
  // Estado actual
  switch (current) {
    case State::MAIN_MENU:
      renderer.drawCentered(boxY + 5, "MENU PRINCIPAL", Color::YELLOW);
      renderer.drawCentered(boxY + 7, "[Q] Salir", Color::WHITE);
      break;
    case State::PAUSE:
      renderer.drawCentered(boxY + 5, "PAUSA", Color::YELLOW);
      renderer.drawCentered(boxY + 7, "[ESC] Volver", Color::WHITE);
      break;
    case State::TOWER:
      renderer.drawCentered(boxY + 5, "TORRE", Color::GREEN);
      renderer.drawCentered(boxY + 7, "[ESC] Pausa", Color::WHITE);
      break;
    case State::HUB:
      renderer.drawCentered(boxY + 5, "HUB", Color::MAGENTA);
      renderer.drawCentered(boxY + 7, "[ESC] Pausa", Color::WHITE);
      break;
    default:
      renderer.drawCentered(boxY + 5, "...", Color::WHITE);
      break;
  }
  
  // Footer con info
  renderer.drawString(2, renderer.getHeight() - 2, 
                     "FPS: 60 | Controles: ESC=Pausa Q=Salir", 
                     Color::BRIGHT_BLACK);
  
  renderer.present();
}

void Game::handleMainMenu() {
  // ToDo implementar menu principal completo
}

void Game::handleTower() {
  // ToDo implementar logica de torre
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
