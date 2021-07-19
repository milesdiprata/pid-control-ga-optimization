#include <iostream>

#include "control/plant_control.h"
#include "control/solver.h"

int main(const int argc, const char* const argv[]) {
  auto constraints = {ga::Gene<double>::Bounds(2, 18),
                      ga::Gene<double>::Bounds(1.05, 9.42),
                      ga::Gene<double>::Bounds(0.26, 2.37)};
  auto solver =
      control::Solver<double, 3>(ga::Procedure<double, 3>::Args(), constraints);
  auto solution = solver.Start();
  std::cout << solution << " " << solution.fitness() << std::endl;

  auto pc = control::PlantControl();
  pc.controller().params().k_p = solution[0].value();
  pc.controller().params().t_i = solution[1].value();
  pc.controller().params().t_d = solution[2].value();
  pc.WriteResponseToFile("a.csv", pc.StepResponse());

  return 0;
}