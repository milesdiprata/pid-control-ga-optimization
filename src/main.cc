#include <iostream>

#include "control/plant_control_system.h"
#include "ga/chromosome.h"
#include "ga/gene.h"
#include "ga/procedure.h"

int main(const int argc, const char* const argv[]) {
  // auto chromosome_template = ga::Chromosome<double, 3>();
  // chromosome_template[0] = ga::Gene<double>(2, 18);
  // chromosome_template[1] = ga::Gene<double>(1.05, 9.42);
  // chromosome_template[2] = ga::Gene<double>(0.26, 2.37);
  // auto p = ga::Procedure<double, 3>(chromosome_template);
  // p.Start();

  auto s = control::PlantControlSystem();
  auto result = s.StepResponse();
  for (const auto& [t, system_output] : result) {
    std::cout << "t=" << t << "\t sys=" << system_output << std::endl;
  }

  return 0;
}