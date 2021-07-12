#include <iostream>

#include "ga/chromosome.h"

int main(const int argc, const char* const argv[]) {
  auto c = ga::Chromosome<double, 3>();
  std::cout << c << std::endl;
  std::cout << "Hello World!\n";
  return 0;
}