#include "UIManager.h"
#include <algorithm>
#include <asm-generic/ioctls.h>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

UIManager::UIManager()
    : playerHealth(100), playerMaxHealth(100), playerMana(50),
      playerMaxMana(50), currentFloor(1), playerLevel(1), dayNightTime(0.0f),
      consoleActive(false), maxConsoleLines(10), historyIndex(-1) {
  registerDefaultCommands();
}

UIManager::~UIManager() {}

void UIManager::setPlayerHealth(int current, int max) {
  playerHealth = current;
  playerMaxHealth = max;
}

void UIManager::setPlayerMana(int current, int max) {
  playerMana = current;
  playerMaxMana = max;
}

void UIManager::setCurrentFloor(int floor) { currentFloor = floor; }

void UIManager::setPlayerLevel(int level) { playerLevel = level; }

void UIManager::setDayNightTime(float time) { dayNightTime = time; }

void UIManager::renderHUD() {
  Renderer &renderer = Renderer::getInstance();
  int width = renderer.getWidth();

  // nivel
  std::string hudText = "Lvl:" + std::to_string(playerLevel);
  renderer.drawString(1, 0, hudText, Color::CYAN + Color::BOLD);
  int xPos = 1 + hudText.length();
  
  renderer.drawString(xPos, 0, " | ", Color::WHITE);
  xPos += 3;

  // vida
  float healthPercent = (float)playerHealth / playerMaxHealth;
  std::string healthColor = Color::GREEN;
  if (healthPercent < 0.3f)
    healthColor = Color::RED;
  else if (healthPercent < 0.6f)
    healthColor = Color::YELLOW;
  
  renderer.drawString(xPos, 0, "HP:", Color::WHITE);
  xPos += 3;
  hudText = std::to_string(playerHealth) + "/" + std::to_string(playerMaxHealth);
  renderer.drawString(xPos, 0, hudText, healthColor + Color::BOLD);
  xPos += hudText.length();
  
  renderer.drawString(xPos, 0, " | ", Color::WHITE);
  xPos += 3;
  
  // Mana
  renderer.drawString(xPos, 0, "MP:", Color::WHITE);
  xPos += 3;
  hudText = std::to_string(playerMana) + "/" + std::to_string(playerMaxMana);
  renderer.drawString(xPos, 0, hudText, Color::BLUE + Color::BOLD);
  xPos += hudText.length();
  
  renderer.drawString(xPos, 0, " | ", Color::WHITE);
  xPos += 3;
  
  // Piso
  renderer.drawString(xPos, 0, "Floor:", Color::WHITE);
  xPos += 6;
  renderer.drawString(xPos, 0, std::to_string(currentFloor), Color::MAGENTA + Color::BOLD);

  // Tiempo (derecha)
  std::string timeStr;
  std::string timeColor;
  if (dayNightTime < 0.5f) {
    int minutes = (int)(dayNightTime * 30.0f);
    timeStr = "DIA " + std::to_string(minutes) + ":00";
    timeColor = Color::YELLOW + Color::BOLD;
  } else {
    int minutes = (int)((dayNightTime - 0.5f) * 30.0f);
    timeStr = "NOCHE " + std::to_string(minutes) + ":00";
    timeColor = Color::BLUE + Color::BOLD;
  }
  
  renderer.drawString(width - timeStr.length() - 2, 0, timeStr, timeColor);

  // Línea separadora
  renderer.drawHLine(0, 1, width, '-', Color::BRIGHT_BLACK);
}

void UIManager::toggleConsole() {
  consoleActive = !consoleActive;
  if (consoleActive) {
    consoleInput.clear();
    historyIndex = -1;
  }
}

bool UIManager::isConsoleActive() const { return consoleActive; }

void UIManager::addConsoleInput(char c) {
  if (c >= 32 && c <= 126) {
    consoleInput += c;
  }
}

void UIManager::removeConsoleInput() {
  if (!consoleInput.empty()) {
    consoleInput.pop_back();
  }
}

void UIManager::submitConsoleCommand() {
  if (consoleInput.empty())
    return;

  consoleHistory.push_back(consoleInput);
  historyIndex = -1;

  log(Color::apply("> " + consoleInput, Color::CYAN));

  executeCommand(consoleInput);

  consoleInput.clear();
}

void UIManager::navigateHistory(bool up) {
  if (consoleHistory.empty())
    return;

  if (up) {
    if (historyIndex == -1) {
      historyIndex = consoleHistory.size() - 1;
    } else if (historyIndex > 0) {
      historyIndex--;
    }
  } else {
    if (historyIndex != -1 && historyIndex < (int)consoleHistory.size() - 1) {
      historyIndex++;
    } else {
      historyIndex = -1;
      consoleInput.clear();
      return;
    }
  }

  if (historyIndex >= 0 && historyIndex < (int)consoleHistory.size()) {
    consoleInput = consoleHistory[historyIndex];
  }
}

void UIManager::clearConsole() {
  consoleOutput.clear();
  log(Color::apply("Console cleared", Color::DARK_GRAY));
}

void UIManager::executeCommand(const std::string &command) {
  auto tokens = parseCommand(command);
  if (tokens.empty())
    return;

  std::string cmd = tokens[0];
  std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

  auto it = commands.find(cmd);
  if (it != commands.end()) {
    it->second(tokens);
  } else {
    logError("Comando desconocido: " + cmd);
    log("Usa 'help' para ver comandos disponibles");
  }
}

std::vector<std::string> UIManager::parseCommand(const std::string &command) {
  std::vector<std::string> tokens;
  std::istringstream iss(command);
  std::string token;

  while (iss >> token) {
    tokens.push_back(token);
  }

  return tokens;
}

void UIManager::registerDefaultCommands() {
  registerCommand("help", [this](const std::vector<std::string> &args) {
    log(Color::apply("=== Comandos Disponibles ===", Color::YELLOW, Color::BOLD));
    log("  help - Muestra esta ayuda");
    log("  clear - Limpia la consola");
    log("  sethealth <valor> - Establece vida actual");
    log("  setmana <valor> - Establece mana actual");
    log("  setfloor <valor> - Establece piso actual");
    log("  setlevel <valor> - Establece nivel del jugador");
    log("  settime <0.0-1.0> - Establece tiempo dia/noche");
    log("  heal - Restaura vida al máximo");
    log("  restoremana - Restaura mana al máximo");
  });

  registerCommand("clear", [this](const std::vector<std::string> &args) {
    clearConsole();
  });

  registerCommand("sethealth",
                  [this](const std::vector<std::string> &args) {
                    if (args.size() < 2) {
                      logError("Uso: sethealth <valor>");
                      return;
                    }
                    try {
                      int value = std::stoi(args[1]);
                      playerHealth = std::min(value, playerMaxHealth);
                      logSuccess("Vida establecida a: " +
                                 std::to_string(playerHealth));
                    } catch (...) {
                      logError("Valor inválido");
                    }
                  });

  registerCommand("setmana", [this](const std::vector<std::string> &args) {
    if (args.size() < 2) {
      logError("Uso: setmana <valor>");
      return;
    }
    try {
      int value = std::stoi(args[1]);
      playerMana = std::min(value, playerMaxMana);
      logSuccess("Mana establecido a: " + std::to_string(playerMana));
    } catch (...) {
      logError("Valor inválido");
    }
  });

  registerCommand("setfloor", [this](const std::vector<std::string> &args) {
    if (args.size() < 2) {
      logError("Uso: setfloor <valor>");
      return;
    }
    try {
      int value = std::stoi(args[1]);
      currentFloor = std::max(1, value);
      logSuccess("Piso establecido a: " + std::to_string(currentFloor));
    } catch (...) {
      logError("Valor inválido");
    }
  });

  registerCommand("setlevel", [this](const std::vector<std::string> &args) {
    if (args.size() < 2) {
      logError("Uso: setlevel <valor>");
      return;
    }
    try {
      int value = std::stoi(args[1]);
      playerLevel = std::max(1, value);
      logSuccess("Nivel establecido a: " + std::to_string(playerLevel));
    } catch (...) {
      logError("Valor inválido");
    }
  });

  registerCommand("settime", [this](const std::vector<std::string> &args) {
    if (args.size() < 2) {
      logError("Uso: settime <0.0-1.0>");
      return;
    }
    try {
      float value = std::stof(args[1]);
      dayNightTime = std::max(0.0f, std::min(1.0f, value));
      logSuccess("Tiempo establecido a: " + std::to_string(dayNightTime));
    } catch (...) {
      logError("Valor inválido");
    }
  });

  registerCommand("heal", [this](const std::vector<std::string> &args) {
    playerHealth = playerMaxHealth;
    logSuccess("Vida restaurada al máximo!");
  });

  registerCommand("restoremana", [this](const std::vector<std::string> &args) {
    playerMana = playerMaxMana;
    logSuccess("Mana restaurado al máximo!");
  });
}

void UIManager::registerCommand(
    const std::string &name,
    std::function<void(const std::vector<std::string> &)> callback) {
  std::string lowerName = name;
  std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                 ::tolower);
  commands[lowerName] = callback;
}

void UIManager::renderConsole() {
  if (!consoleActive)
    return;

  Renderer &renderer = Renderer::getInstance();
  int width = renderer.getWidth();
  int height = renderer.getHeight();

  int consoleHeight = height / 2;
  int startY = height - consoleHeight;

  // Borde superior
  renderer.drawChar(0, startY, '+', Color::CYAN + Color::BOLD);
  renderer.drawHLine(1, startY, width - 2, '=', Color::CYAN + Color::BOLD);
  renderer.drawChar(width - 1, startY, '+', Color::CYAN + Color::BOLD);
  
  // Título
  std::string title = "[ DEBUG CONSOLE ]";
  int titleX = (width - title.length()) / 2;
  renderer.drawString(titleX, startY, title, Color::CYAN + Color::BOLD);

  // Área de salida
  int outputStartY = startY + 1;
  int outputLines = consoleHeight - 3;

  int startIdx = std::max(0, (int)consoleOutput.size() - outputLines);
  for (int i = 0; i < outputLines && (startIdx + i) < (int)consoleOutput.size(); i++) {
    renderer.drawString(1, outputStartY + i, consoleOutput[startIdx + i], Color::WHITE);
  }

  // Borde inferior
  int inputY = height - 1;
  renderer.drawChar(0, inputY, '+', Color::CYAN);
  renderer.drawHLine(1, inputY, width - 2, '=', Color::CYAN);
  renderer.drawChar(width - 1, inputY, '+', Color::CYAN);

  // Input
  renderer.drawString(1, inputY, "> ", Color::GREEN + Color::BOLD);
  renderer.drawString(3, inputY, consoleInput, Color::WHITE);
  renderer.drawChar(3 + consoleInput.length(), inputY, '_', Color::GREEN);
}

void UIManager::log(const std::string &message) {
  consoleOutput.push_back(message);
  
  while ((int)consoleOutput.size() > 100) {
    consoleOutput.erase(consoleOutput.begin());
  }
}

void UIManager::logError(const std::string &message) {
  log(Color::apply("[ERROR] " + message, Color::RED, Color::BOLD));
}

void UIManager::logSuccess(const std::string &message) {
  log(Color::apply("[OK] " + message, Color::GREEN, Color::BOLD));
}
