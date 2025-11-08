#include "GameState.h"

GameState::GameState()
    : currentState(State::MAIN_MENU), previousState(State::MAIN_MENU) {}

void GameState::setState(State newState) {
  previousState = currentState;
  currentState = newState;
}

State GameState::getState() const { return currentState; }

State GameState::getPreviousState() const { return previousState; }

void GameState::returnToPreviousState() {
  State temp = currentState;
  currentState = previousState;
  previousState = temp;
}

bool GameState::isInTower() const { return currentState == State::TOWER; }

bool GameState::isInHub() const { return currentState == State::HUB; }

bool GameState::isInCombat() const { return currentState == State::COMBAT; }

bool GameState::isInMenu() const {
  return currentState == State::MAIN_MENU || currentState == State::PAUSE ||
         currentState == State::INVENTORY;
}
