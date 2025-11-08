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

  // inicializar input en modo raw+
  Input::getInstance().enableRawMode();

  // limpiar pantalla
  system("clear");

  std::cout << Color::BRIGHT_CYAN
            << "=== THE INFINITE SPIRE ===" << Color::RESET << std::endl;

  std::cout << Color::YELLOW << "inicializando..." << Color::RESET << std::endl;

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
  // Mover cursor al inicio
  std::cout << "\033[H";
  
  State current = gameState.getState();
  
  // Header
  std::cout << Color::BRIGHT_CYAN << "=== THE INFINITE SPIRE ===" 
            << Color::RESET << std::endl;
  std::cout << Color::YELLOW << "Estado: ";
  
  switch (current) {
    case State::MAIN_MENU:
      std::cout << "MENU PRINCIPAL" << Color::RESET << std::endl;
      std::cout << "\n[Q] Salir" << std::endl;
      break;
    case State::PAUSE:
      std::cout << "PAUSA" << Color::RESET << std::endl;
      std::cout << "\n[ESC] Volver" << std::endl;
      break;
    case State::TOWER:
      std::cout << "TORRE" << Color::RESET << std::endl;
      break;
    case State::HUB:
      std::cout << "HUB" << Color::RESET << std::endl;
      break;
    default:
      std::cout << "..." << Color::RESET << std::endl;
      break;
  }
  
  std::cout << std::flush;
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
  Input::getInstance().disableRawMode();
  system("clear");
  std::cout << Color::BRIGHT_MAGENTA << "Gracias por jugar" << Color::RESET
            << std::endl;
}

void Game::quit() { running = false; }

bool Game::isRunning() const { return running; }
