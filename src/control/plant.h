#ifndef CONTROL_PLANT_H_
#define CONTROL_PLANT_H_

#include "control/system.h"

namespace control {

class Plant : public System {
 public:
  static constexpr const double kEpsilon = 0.02;

  constexpr Plant() = default;
  virtual constexpr ~Plant() = default;

  constexpr void reset() final { System::reset(); }

 protected:
  constexpr const double Transform(const double input) final {
    return ((kSampleTimeSecs * input) + output()) /
           (1.0 + (kEpsilon * kSampleTimeSecs));
  }
};

}  // namespace control

#endif  // CONTROL_PLANT_H_
