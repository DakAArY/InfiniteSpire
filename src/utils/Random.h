#ifndef RANDOM_H
#define RANDOM_H

#include <ctime>
#include <random>
#include <string>

class Random {
private:
  std::mt19937 generator;
  static Random *instance;

  Random();

public:
  static Random &getInstance();

  // Establecimiento de la semilla desde el sistema
  void setSeedFromDate();

  // Establecimiento manual de la semilla
  void setSeed(unsigned int seed);

  // Genera entero en rango [min, max]
  int getInt(int min, int max);

  // Genera float en rango [0.0, 1.0]
  float getFloat();

  // Genera float en rango [min, max]
  float getFloat(float min, float max);

  // Probabilidad (0.0 a 1.0) - basicamente true si random <) probability
  bool chance(float probaility);

  // obtiene semilla actual
  unsigned int getCurrentSeed();

private:
  unsigned int generateSeedFromDate();
  unsigned int currentSeed;
};

#endif
