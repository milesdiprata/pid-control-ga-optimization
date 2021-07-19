#include <iostream>

#include "control/solver.h"

int main(const int argc, const char* const argv[]) {
  auto constraints = {ga::Gene<double>::Bounds(2, 18),
                      ga::Gene<double>::Bounds(1.05, 9.42),
                      ga::Gene<double>::Bounds(0.26, 2.37)};
  auto solver =
      control::Solver<double, 3>(ga::Procedure<double, 3>::Args(), constraints);
  auto solution = solver.Start();
  std::cout << solution << " " << solution.fitness() << std::endl;
  return 0;
}