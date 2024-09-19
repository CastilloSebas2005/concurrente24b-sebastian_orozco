// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <chrono>
#include <mutex>
#include <random>
#include <thread>

#include "Util.hpp"

// Try to get a seed from hardware, if available
static std::random_device::result_type seed = std::random_device()();
// This object generates random numbers using the Mersenne-Twister algorithm
static std::mt19937 randomEngine(seed);
// Protects the random engine to be thread-safe
static std::mutex canAccessRandomEngine;

int Util::random(int min, int max) {
  // Produce random values with uniform discrete distribution
  std::uniform_int_distribution<int> randomDistribution(min, max - 1);
  // Generate and return a random number using the uniform distribution
  canAccessRandomEngine.lock();
  const int result = randomDistribution(randomEngine);
  canAccessRandomEngine.unlock();
  return result;
}

double Util::random(double min, double max) {
  // Produce random values with uniform discrete distribution
  std::uniform_real_distribution<double> randomDistribution(min, max);
  // Generate and return a random number using the uniform distribution
  canAccessRandomEngine.lock();
  const double result = randomDistribution(randomEngine);
  canAccessRandomEngine.unlock();
  return result;
}

void Util::sleepFor(int milliseconds) {
  // If a negative number of milliseconds is given, sleep for a random number
  // of milliseconds in range [0, milliseconds[
  if (milliseconds < 0) {
    milliseconds = Util::random(0, std::abs(milliseconds));
  }

  // Sleep for the given amount of milliseconds
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

std::vector<std::string> Util::split(const std::string& text,
  const std::string& delim, bool trimEmpty) {
  std::vector<std::string> tokens;
  // Util::tokenize(text, tokens, static_cast<size_t(std::string::*)(const
  // std::string&, size_t)>(&std::string::find_first_of), delim, trimEmpty);
  Util::tokenize(text, tokens, delim, trimEmpty);
  return tokens;
}
