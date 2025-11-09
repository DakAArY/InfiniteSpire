#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>

class Renderer {
private:
  static Renderer *instance;

  int width;
  int height;
  std::vector<std::vector<char>> buffer;
  std::vector<std::vector<std::string>> colorBuffer;

  Renderer();

  void initBuffers();
  void clearBuffers();

public:
  static Renderer &getInstance();

  // inicializar dimensiones de terminal
  void initialize();

  // limpiar pantalla completa
  void clear();

  // dibujar caracter en posicion con color
  void drawChar(int x, int y, char c, const std::string &color = "");

  // dibujar string en posicion con color
  void drawString(int x, int y, const std::string &str,
                  const std::string &color = "");

  // dibujar linea horizontal
  void drawHLine(int x, int y, int lenght, char c = '-',
                 const std::string &color = "");

  // dibujar linea vertical
  void drawVLine(int x, int y, int lenght, char c = '|',
                 const std::string &color = "");

  // dibujar caja/rectangulo
  void drawBox(int x, int y, int width, int height,
               const std::string &color = "");

  // centrar texto en una linea
  void drawCentered(int y, const std::string &str,
                    const std::string &color = "");

  // renderizar buffer a pantalla
  void present();

  // getters
  int getWidth() const;
  int getHeight() const;

  // ocultar/mostrar cursor
  void hideCursor();
  void showCursor();

  // mover cursor a posicion
  void moveCursor(int x, int y);
};

#endif
