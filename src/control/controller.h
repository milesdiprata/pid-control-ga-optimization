#ifndef CONTROL_CONTROLLER_H_
#define CONTROL_CONTROLLER_H_

#include <cstddef>

#include "control/system.h"

namespace control {

class Controller : public System {
 public:
  struct Parameters {
    static constexpr const double kDefaultKp = 2.0;
    static constexpr const double kDefaultKi = 1.05;
    static constexpr const double kDefaultKd = 0.26;
    static constexpr const double kDefaultTau = 0.0;

    constexpr Parameters(const double k_p = kDefaultKp,
                         const double k_i = kDefaultKi,
                         const double k_d = kDefaultKd,
                         const double tau = kDefaultTau)
        : k_p(k_p), k_i(k_i), k_d(k_d), tau(tau) {}

    constexpr ~Parameters() = default;

    double k_p;
    double k_i;
    double k_d;
    double tau;
  };

  static constexpr const int kNumParams = 3;

  static constexpr const double kOutputMin = -10.0;
  static constexpr const double kOutputMax = 10.0;

  static constexpr const double kUnitStepSetPoint = 1.0;

  constexpr Controller(const double setpoint = kUnitStepSetPoint)
      : System(), setpoint_(setpoint) {}

  constexpr Controller(const double k_p, const double t_i, const double t_d)
      : System(), params_(k_p, k_p / t_i, (k_p * t_d) / kSampleTimeSecs) {}

  virtual constexpr ~Controller() = default;

  constexpr const Parameters& params() const { return params_; }
  constexpr const Parameters& params() { return params_; }

  constexpr void reset() override {
    System::reset();
    integrator_ = 0.0;
    differentiator_ = 0.0;
    prev_error_ = 0.0;
    prev_measurement_ = 0.0;
  }

 protected:
  constexpr const double Transform(const double measurement) override {
    double error = setpoint_ - measurement;

    double proportional = params_.k_p * error;

    integrator_ += 0.5 * params_.k_i * kSampleTimeSecs * (error + prev_error_);

    double integrator_min =
        kOutputMin < proportional ? kOutputMin - proportional : 0.0;
    double integrator_max =
        kOutputMax > proportional ? kOutputMax - proportional : 0.0;

    if (integrator_ < integrator_min) {
      integrator_ = integrator_min;
    } else if (integrator_ > integrator_max) {
      integrator_ = integrator_max;
    }

    differentiator_ =
        -((2.0 * params_.k_d * (measurement - prev_measurement_)) +
          ((2.0 * params_.tau - kSampleTimeSecs) * differentiator_)) /
        (2.0 * params_.tau + kSampleTimeSecs);

    double output = proportional + integrator_ + differentiator_;
    if (output < kOutputMin) {
      output = kOutputMin;
    } else if (output > kOutputMax) {
      output = kOutputMax;
    }

    prev_error_ = error;
    prev_measurement_ = measurement;

    return output;
  }

  Parameters params_;

  double setpoint_;
  double integrator_;
  double differentiator_;
  double prev_error_;
  double prev_measurement_;
};

}  // namespace control

#endif  // CONTROL_CONTROLLER_H_