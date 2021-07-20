#ifndef CONTROL_CONTROLLER_H_
#define CONTROL_CONTROLLER_H_

#include <cstddef>

#include "control/system.h"

namespace control {

class Controller : public System {
 public:
  struct Parameters {
    static constexpr const double kDefaultKp = 2.0;
    static constexpr const double kDefaultTi = 1.05;
    static constexpr const double kDefaultTd = 0.26;
    static constexpr const double kDefaultTau = 0.02;

    constexpr Parameters(const double k_p = kDefaultKp,
                         const double t_i = kDefaultTi,
                         const double t_d = kDefaultTd,
                         const double tau = kDefaultTau)
        : k_p(k_p), t_i(t_i), t_d(t_d), tau(tau) {}

    constexpr ~Parameters() = default;

    constexpr const double k_i() const { return k_p / t_i; }
    constexpr const double k_d() const { return k_p * t_d; }

    double k_p;
    double t_i;
    double t_d;
    double tau;
  };

  static constexpr const int kNumParams = 3;

  static constexpr const double kOutputMin = -10.0;
  static constexpr const double kOutputMax = 10.0;

  static constexpr const double kUnitStepSetPoint = 1.0;

  constexpr Controller(const double setpoint = kUnitStepSetPoint)
      : System(), setpoint_(setpoint) {}

  constexpr Controller(const double k_p, const double t_i, const double t_d)
      : System(), params_(k_p, t_i, t_d) {}

  virtual constexpr ~Controller() = default;

  constexpr const Parameters& params() const { return params_; }
  constexpr Parameters& params() { return params_; }

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

    integrator_ = integrator_ +
                  0.5 * params_.k_i() * kSampleTimeSecs * (error + prev_error_);

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
        -((2.0 * params_.k_d() * (measurement - prev_measurement_)) +
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