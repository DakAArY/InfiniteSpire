#ifndef COLOR_H
#define COLOR_H

#include <string>

namespace Color {
// Codigos ANSI para colores
const std::string RESET = "\033[0m";

// colores basicos
const std::string BLACK = "\033[30m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";

// colores brillatens
const std::string BRIGHT_BLACK = "\033[90m";
const std::string BRIGHT_RED = "\033[91m";
const std::string BRIGHT_GREEN = "\033[92m";
const std::string BRIGHT_YELLOW = "\033[93m";
const std::string BRIGHT_BLUE = "\033[94m";
const std::string BRIGHT_MAGENTA = "\033[95m";
const std::string BRIGHT_CYAN = "\033[96m";
const std::string BRIGHT_WHITE = "\033[97m";

// estilos de texto
const std::string BOLD = "\033[1m";
const std::string DIM = "\033[2m";
const std::string UNDERLINE = "\033[4m";

// colores para rerezas
const std::string COMMON = WHITE;
const std::string RARE = BLUE;
const std::string EPIC = MAGENTA;
const std::string LEGENDARY = YELLOW;

// funciones helper
std::string colorize(const std::string &text, const std::string &color);
std::string getRandomWallColor();
std::string
getRarityColor(int rarity); // 0=comun, 1=raro, 2=epico, 3=legendario

// limpiar pantall
void clearScreen();

// mover cursor
void moveCursor(int x, int y);

// ocultar o mostrar cursor
void hideCursor();
void showCursor();
} // namespace Color

#endif
