#ifndef PID_SOLVER_H_
#define PID_SOLVER_H_

#include <cstddef>
#include <limits>

#include "control/controller.h"
#include "control/plant_control.h"
#include "ga/chromosome.h"
#include "ga/procedure.h"

namespace control {

template <typename T = double, std::size_t N = control::Controller::kNumParams>
class Solver : public ga::Procedure<T, N> {
 public:
  static constexpr const double kMaxFitnessValue = 1000.0;

  static constexpr const double kIntegralSquaredErrorWeight = 0.25;
  static constexpr const double kRiseTimeWeight = 0.25;
  static constexpr const double kSettlingTimeWeight = 0.25;
  static constexpr const double kMaxOvershootWeight = 0.25;

  constexpr Solver(const typename ga::Procedure<T, N>::Args& args,
                   const std::vector<typename ga::Gene<T>::Bounds>& constraints)
      : ga::Procedure<T, N>(args, constraints) {}

  virtual constexpr ~Solver() = default;

 protected:
  constexpr const double Fitness(const ga::Chromosome<T, N>& chromosome) final {
    plant_control_.controller().params().k_p = chromosome[0].value();
    plant_control_.controller().params().t_i = chromosome[1].value();
    plant_control_.controller().params().t_d = chromosome[2].value();

    auto response = plant_control_.StepResponse();
    if (!response.rise_time.has_value() ||
        !response.settling_time.has_value() ||
        !response.max_overshoot.has_value()) {
      return kMaxFitnessValue;
    } else {
      return (kIntegralSquaredErrorWeight *
              plant_control_.IntegralSquaredError(response)) +
             (kRiseTimeWeight * response.rise_time.value()) +
             (kSettlingTimeWeight * response.settling_time.value()) +
             (kMaxOvershootWeight * response.max_overshoot.value());
    }
  }

 private:
  PlantControl plant_control_;
};

}  // namespace control

#endif  // PID_GA_SOLVER_H_
