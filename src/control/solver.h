#ifndef PID_SOLVER_H_
#define PID_SOLVER_H_

#include <cstddef>

#include "control/controller.h"
#include "control/plant_control.h"
#include "ga/chromosome.h"
#include "ga/procedure.h"

namespace control {

template <typename T = double, std::size_t N = control::Controller::kNumParams>
class Solver : public ga::Procedure<T, N> {
 public:
  static constexpr const double kIntegralSquaredErrorWeight = 0.25;
  static constexpr const double kRiseTimeWeight = 0.25;
  static constexpr const double kSettlingTimeWeight = 0.25;
  static constexpr const double kMaxOvershootWeight = 0.25;

  constexpr Solver(const typename ga::Procedure<T, N>::Args& args =
                       ga::Procedure<T, N>::Args())
      : ga::Procedure<T, N>(args) {}

  constexpr Solver(const ga::Chromosome<T, N>& chromosome_template)
      : ga::Procedure<T, N>(chromosome_template) {}

  constexpr Solver(const typename ga::Procedure<T, N>::Args& args,
                   const ga::Chromosome<T, N>& chromosome_template)
      : ga::Procedure<T, N>(args, chromosome_template) {}

  virtual constexpr ~Solver() = default;

 protected:
  constexpr const double Fitness(
      const ga::Chromosome<T, N>& chromosome) const final {
    // return (kIntegralSquaredErrorWeight *
    //         plant_control_.IntegralSquaredError(plant_control_.S))
    return 1.0;
  }

 private:
  ga::Chromosome<T, N> solution_;
  PlantControl plant_control_;
};

}  // namespace control

#endif  // PID_GA_SOLVER_H_
