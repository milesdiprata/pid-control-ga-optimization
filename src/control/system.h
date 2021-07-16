#ifndef CONTROL_SYSTEM_H_
#define CONTROL_SYSTEM_H_

#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <vector>

namespace control {

class System {
 public:
  struct Response {
    struct TimeValue {
      constexpr TimeValue(const double time = 0.0, const double value = 0.0)
          : time(time), value(value) {}

      constexpr ~TimeValue() = default;

      double time;
      double value;
    };

    Response() : max_overshoot(std::numeric_limits<double>::min()) {}
    ~Response() = default;

    std::vector<TimeValue> time_values;
    std::optional<double> rise_time;
    std::optional<double> settling_time;
    double max_overshoot;
  };

  static constexpr const double kSimulationTimeSecs = 15.0;
  static constexpr const double kSampleTimeSecs = 0.01;

  constexpr System() = default;
  virtual constexpr ~System() = default;

  virtual constexpr void reset() { output_ = 0; }

  constexpr const double Update(const double input) {
    output_ = Transform(input);
    return output_;
  }

  static void WriteResponseToFile(const std::string& file_name,
                                  const Response& response) {
    auto csv_file = std::ofstream(file_name.c_str(), std::fstream::out);

    csv_file << "value,time\n";
    for (const auto& [time, value] : response.time_values) {
      csv_file << value << "," << time << "\n";
    }

    csv_file.close();
  }

 protected:
  constexpr const double output() const { return output_; }

  virtual constexpr const double Transform(const double input) = 0;

 private:
  double output_;
};

std::ostream& operator<<(std::ostream& os, const System::Response& response) {
  os << "Rise time:\t"
     << response.rise_time.value_or(std::numeric_limits<double>::max()) << "\n";

  os << "Settling time:\t"
     << response.settling_time.value_or(std::numeric_limits<double>::max())
     << "\n";

  os << "Max overshoot:\t" << response.max_overshoot;

  return os;
}

}  // namespace control

#endif  // CONTROL_SYSTEM_H_