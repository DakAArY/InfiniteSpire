#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>

class Menu {
private:
  std::vector<std::string> options;
  int selectedIndex;
  std::string title;
  int x, y; // posicion del menu
  int width;

public:
  Menu(const std::string &title, const std::vector<std::string> &options);

  // navegacion
  void moveUp();
  void moveDown();

  // obtener seleccion actual
  int getSelectedIndex() const;
  std::string getSelectedOption() const;

  // renderizar menu
  void render();

  // setters para posicion
  void setPosition(int posX, int posY);
  void setWidth(int w);
};

#endif
