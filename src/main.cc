#include <iostream>

#include "control/solver.h"

int main(const int argc, const char* const argv[]) {
  auto constraints = {ga::Gene<double>::Bounds(2, 18),
                      ga::Gene<double>::Bounds(1.05, 9.42),
                      ga::Gene<double>::Bounds(0.26, 2.37)};
  auto s =
      control::Solver<double, 3>(ga::Procedure<double, 3>::Args(), constraints);
  s.Start();

  // auto s = control::PlantControl();
  // auto response = s.StepResponse();
  // s.WriteResponseToFile("a.csv", response);

  // std::cout << response << std::endl;
  // std::cout << "ISE:\t\t" << s.IntegralSquaredError(response) << std::endl;
  return 0;
}