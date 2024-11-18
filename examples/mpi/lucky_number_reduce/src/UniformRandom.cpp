#include <cstdlib>
#include <iostream>

#include "UniformRandom.hpp"

#ifdef UNIFORM_RANDOM

// usage UniformRandom COUNT MIN MAX
int main(int argc, char* argv[]) {
  const int count = argc >= 2 ? std::atoi(argv[1]) : 1;
  const int min = argc >= 3 ? std::atoi(argv[2]) : 0;
  const int max = argc >= 4 ? std::atoi(argv[3]) : 100;

  // Generate a pseudo-random numbers
  UniformRandom<int> uniformRandom;
  for (int current = 0; current < count; ++current) {
    std::cout << uniformRandom.between(min, max) << std::endl;
  }
}

#endif
