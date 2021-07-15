#ifndef CONTROL_SYSTEM_H_
#define CONTROL_SYSTEM_H_

#include <cstddef>
#include <vector>

namespace control {

class System {
 public:
  struct TimeValue {
    constexpr TimeValue(const double time = 0.0, const double value = 0.0)
        : time(time), value(value) {}

    constexpr ~TimeValue() = default;

    double time;
    double value;
  };

  static constexpr const double kSimulationTimeSecs = 50.0;
  static constexpr const double kSampleTimeSecs = 0.01;

  constexpr System() = default;
  virtual constexpr ~System() = default;

  virtual constexpr void reset() { output_ = 0; }

  constexpr const double Update(const double input) {
    output_ = Transform(input);
    return output_;
  }

 protected:
  constexpr const double output() const { return output_; }

  virtual constexpr const double Transform(const double input) = 0;

 private:
  double output_;
};

}  // namespace control

#endif  // CONTROL_SYSTEM_H_
