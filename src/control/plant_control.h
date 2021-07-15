#ifndef CONTROL_PLANT_CONTROL_H_
#define CONTROL_PLANT_CONTROL_H_

#include <cstddef>
#include <utility>
#include <vector>

#include "control/controller.h"
#include "control/plant.h"
#include "control/system.h"

namespace control {

class PlantControl : public System {
 public:
  static constexpr const double kAlpha = 0.02;

  constexpr PlantControl() = default;
  virtual constexpr ~PlantControl() = default;

  constexpr void reset() final {
    System::reset();
    controller_.reset();
    plant_.reset();
  }

  const std::vector<TimeValue> StepResponse() {
    reset();

    auto step_response = std::vector<TimeValue>();
    for (double t = 0.0; t <= kSimulationTimeSecs; t += kSampleTimeSecs) {
      double measurement = Update(plant_.Update(controller_.Update(output())));
      step_response.push_back(TimeValue(t, measurement));
    }

    return step_response;
  }

  static constexpr const double IntegralSquaredError(
      const std::vector<TimeValue>& response) {
    double ise = 0.0;
    for (std::size_t i = 0, size = response.size() - 1; i < size; ++i) {
      double error = Controller::kUnitStepSetPoint - response[i].value;
      ise += (error * error) * (response[i + 1].value - response[i].value);
    }

    return ise;
  }

 protected:
  // Simulates dynamics of a first-order system
  constexpr const double Transform(const double input) final {
    return (kSampleTimeSecs * input + output()) /
           (1.0 + kAlpha * kSampleTimeSecs);
  }

 private:
  Controller controller_;
  Plant plant_;
};

}  // namespace control

#endif  // CONTROL_PLANT_CONTROL_SYSTEM_H_