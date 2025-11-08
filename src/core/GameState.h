#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <cstdlib>
enum class State {
  MAIN_MENU,
  CHARACTER_CREATION,
  TOWER,
  HUB,
  COMBAT,
  INVENTORY,
  MERCHANT,
  BLACKSMITH,
  INN,
  SANCTUARY,
  PAUSE,
  GAME_OVER,
  EXIT
};

class GameState {
private:
  State currentState;
  State previousState;

public:
  GameState();

  // cambiar estado
  void setState(State newState);

  // obtener estado actual
  State getState() const;

  // Obtener estado anterior
  State getPreviousState() const;

  // regresar al estado anterior
  void returnToPreviousState();

  // verificaciones rapidas
  bool isInTower() const;
  bool isInHub() const;
  bool isInCombat() const;
  bool isInMenu() const;
};

#endif
