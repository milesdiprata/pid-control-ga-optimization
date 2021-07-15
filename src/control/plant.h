#ifndef CONTROL_PLANT_H_
#define CONTROL_PLANT_H_

#include "control/system.h"

namespace control {

class Plant : public System {
 public:
  static constexpr const double kAlpha = 1;

  constexpr Plant() = default;
  virtual constexpr ~Plant() = default;

 protected:
  // https://electronics.stackexchange.com/questions/52099/implementing-a-continuous-time-transfer-function-in-pc-code
  //        H(s) = 1 / ((alpha * s) + 1)
  // => y[n + 1] = y[n] + T * (1 / alpha) * (x[n] - y[n])
  constexpr const double Transform(const double input) final {
    double output = System::output();
    return output + (kSampleTimeSecs / kAlpha) * (input - output);
  }
};

}  // namespace control

#endif  // CONTROL_PLANT_H_