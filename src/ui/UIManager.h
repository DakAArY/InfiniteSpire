#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "../utils/Color.h"
#include "Renderer.h"
#include <functional>
#include <map>
#include <numeric>
#include <string>
#include <vector>

class UIManager {
private:
  // stats del jugador para el HUd
  int playerHealth;
  int playerMaxHealth;
  int playerMana;
  int playerMaxMana;
  int currentFloor;
  int playerLevel;
  float dayNightTime; // 0.0 - 1.0

  // debug console
  bool consoleActive;
  std::string consoleInput;
  std::vector<std::string> consoleHistory;
  std::vector<std::string> consoleOutput;
  int maxConsoleLines;
  int historyIndex;

  // comandos registrados
  std::map<std::string, std::function<void(const std::vector<std::string> &)>>
      commands;

  // metodos internos
  void executeCommand(const std::string &command);
  std::vector<std::string> parseCommand(const std::string &command);
  void registerDefaultCommands();

public:
  UIManager();
  ~UIManager();

  // actualizar stats del HUD
  void setPlayerHealth(int current, int max);
  void setPlayerMana(int current, int max);
  void setCurrentFloor(int floor);
  void setPlayerLevel(int level);
  void setDayNightTime(float time);

  // renderizar el hud
  void renderHUD();

  // debug console
  void toggleConsole();
  bool isConsoleActive() const;
  void addConsoleInput(char c);
  void removeConsoleInput();
  void submitConsoleCommand();
  void navigateHistory(bool up);
  void clearConsole();

  // registrar comando personalizado
  void registerCommand(
      const std::string &name,
      std::function<void(const std::vector<std::string> &)> callback);

  // renderizar la consola
  void renderConsole();

  // log a consola
  void log(const std::string &message);
  void logError(const std::string &message);
  void logSuccess(const std::string &message);
};

#endif
