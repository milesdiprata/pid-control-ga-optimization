#ifndef CONTROL_PLANT_CONTROL_H_
#define CONTROL_PLANT_CONTROL_H_

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>

#include "control/controller.h"
#include "control/plant.h"
#include "control/system.h"

namespace control {

class PlantControl : public System {
 public:
  static constexpr const double kRiseTimeThreshold = 0.9;

  static constexpr const double kSteadyStateErrorPercent = 0.01;
  static constexpr const double kSteadyStateThreshold =
      Controller::kUnitStepSetPoint * kSteadyStateErrorPercent;

  constexpr PlantControl() = default;
  virtual constexpr ~PlantControl() = default;

  constexpr const Controller& controller() const { return controller_; }
  constexpr Controller& controller() { return controller_; }

  constexpr const Plant& plant() const { return plant_; }
  constexpr Plant& plant() { return plant_; }

  constexpr void reset() final {
    System::reset();
    controller_.reset();
    plant_.reset();
  }

  const Response StepResponse() {
    reset();

    auto response = Response();

    for (double time = 0.0; time <= kSimulationTimeSecs;
         time += kSampleTimeSecs) {
      double measurement = update_output(output());
      response.time_values.push_back(Response::TimeValue(time, measurement));

      if (!response.rise_time.has_value() &&
          std::round(measurement * 100.0) / 100.0 == kRiseTimeThreshold) {
        response.rise_time = time;
      }

      double abs_error = std::fabs(Controller::kUnitStepSetPoint - measurement);
      if (!response.settling_time.has_value() &&
          abs_error < kSteadyStateThreshold) {
        response.settling_time = time;
      } else if (response.settling_time.has_value() &&
                 abs_error > kSteadyStateThreshold) {
        response.settling_time.reset();
      }

      if (measurement >= Controller::kUnitStepSetPoint) {
        response.max_overshoot =
            response.max_overshoot.has_value()
                ? std::max(response.max_overshoot.value(), measurement)
                : measurement;
      }
    }

    return response;
  }

  static constexpr const double IntegralSquaredError(const Response& response) {
    auto& time_values = response.time_values;
    double ise = 0.0;
    for (std::size_t i = 1, size = time_values.size(); i < size; ++i) {
      double error = Controller::kUnitStepSetPoint - time_values[i].value;
      ise += (error * error) * (time_values[i].time - time_values[i - 1].time);
    }

    return ise;
  }

 protected:
  constexpr const double Transform(const double input) final {
    return plant_.update_output(controller_.update_output(input));
  }

 private:
  Controller controller_;
  Plant plant_;
};

}  // namespace control

#endif  // CONTROL_PLANT_CONTROL_SYSTEM_H_
