#include <iostream>

#include "ga/procedure.h"

int main(const int argc, const char* const argv[]) {
  auto p = ga::Procedure<double, 3>();
  p.Start();
  return 0;
}