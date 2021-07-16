#include <iostream>

#include "control/solver.h"

int main(const int argc, const char* const argv[]) {
  auto chromosome_template = ga::Chromosome<double, 3>();
  chromosome_template[0] = ga::Gene<double>(2, 18);
  chromosome_template[1] = ga::Gene<double>(1.05, 9.42);
  chromosome_template[2] = ga::Gene<double>(0.26, 2.37);
  auto s = control::Solver<>(chromosome_template);
  s.Start();

  // auto s = control::PlantControl();
  // auto response = s.StepResponse();
  // s.WriteResponseToFile("a.csv", response);

  // std::cout << response << std::endl;
  // std::cout << "ISE:\t\t" << s.IntegralSquaredError(response) << std::endl;
  return 0;
}