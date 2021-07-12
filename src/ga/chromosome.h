#ifndef GA_CHROMOSOME_H_
#define GA_CHROMOSOME_H_

#include <array>
#include <cstddef>
#include <iostream>
#include <string>

namespace ga {

template <typename T, std::size_t N>
class Chromosome;

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const Chromosome<T, N>& chromosome);

template <typename T, std::size_t N>
class Chromosome {
 public:
  Chromosome();
  ~Chromosome();

  inline const std::array<T, N>& paramaters() const { return parameters_; }
  inline std::array<T, N>& paramaters() { return parameters_; }

  friend std::ostream& operator<<<T, N>(std::ostream& os,
                                        const Chromosome& chromosome);

 private:
  std::array<T, N> parameters_;
};

template <typename T, std::size_t N>
Chromosome<T, N>::Chromosome() {}

template <typename T, std::size_t N>
Chromosome<T, N>::~Chromosome() {}

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const Chromosome<T, N>& chromosome) {
  std::string seperator = "";
  os << "<";

  for (const auto& parameter : chromosome.parameters_) {
    std::cout << seperator << parameter;
    seperator = ", ";
  }

  os << ">";

  return os;
}

}  // namespace ga

#endif  // GA_CHROMOSOME_H_