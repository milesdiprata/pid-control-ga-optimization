#ifndef CONTROL_PLANT_CONTROL_SYSTEM_H_
#define CONTROL_PLANT_CONTROL_SYSTEM_H_

#include <utility>
#include <vector>

#include "control/controller.h"
#include "control/system.h"

namespace control {

class PlantControlSystem : System {
 public:
  static constexpr const double kAlpha = 0.02;

  constexpr PlantControlSystem() = default;
  virtual constexpr ~PlantControlSystem() = default;

  constexpr void reset() final {
    System::reset();
    controller_.reset();
  }

  const std::vector<std::pair<double, double>> StepResponse() {
    reset();

    auto step_response = std::vector<std::pair<double, double>>();
    for (double t = 0.0; t <= kSimulationTimeSecs; t += kSampleTimeSecs) {
      double measurement = Update(controller_.Update(output()));

      step_response.push_back(std::make_pair(t, measurement));
    }

    return step_response;
  }

 protected:
  constexpr const double Transform(const double input) final {
    return (kSampleTimeSecs * input + output()) /
           (1.0 + kAlpha * kSampleTimeSecs);
  }

 private:
  Controller controller_;
};

}  // namespace control

#endif  // CONTROL_PLANT_CONTROL_SYSTEM_H_