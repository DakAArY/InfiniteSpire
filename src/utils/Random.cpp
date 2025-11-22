#include "Random.h"
#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>

Random *Random::instance = nullptr;

Random::Random() : currentSeed(0) { setSeedFromDate(); }

Random &Random::getInstance() {
  if (instance == nullptr) {
    instance = new Random();
  }
  return *instance;
}

unsigned int Random::generateSeedFromDate() {
  std::time_t now = std::time(nullptr);
  std::tm *local = std::localtime(&now);

  // Formato: DDMMYYYY como numero
  unsigned int seed = (local->tm_mday * 1000000) +
                      ((local->tm_mon + 1)) * (10000) + (local->tm_year + 1900);
  return seed;
}

void Random::setSeedFromDate() {
  currentSeed = generateSeedFromDate();
  generator.seed(currentSeed);
}

void Random::setSeed(unsigned int seed) {
  currentSeed = seed;
  generator.seed(seed);
}

int Random::getInt(int min, int max) {
  std::uniform_int_distribution<int> distribution(min, max);
  return distribution(generator);
}

float Random::getFloat() {
  std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
  return distribution(generator);
}

float Random::getFloat(float min, float max) {
  std::uniform_real_distribution<float> distribution(min, max);
  return distribution(generator);
}

bool Random::chance(float probability) { return getFloat() <= probability; }

unsigned int Random::getCurrentSeed() { return currentSeed; }
