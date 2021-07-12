#ifndef GA_CHROMOSOME_H_
#define GA_CHROMOSOME_H_

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace ga {

template <typename T, std::size_t N>
struct Chromosome;

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const Chromosome<T, N>& chromosome);

template <typename T, std::size_t N>
struct Chromosome {
 public:
  Chromosome();
  Chromosome(const Chromosome& chromosome);
  Chromosome(Chromosome&& chromosome);
  ~Chromosome();

  std::vector<T> params;
};

template <typename T, std::size_t N>
Chromosome<T, N>::Chromosome() : params(N) {}

template <typename T, std::size_t N>
Chromosome<T, N>::Chromosome(const Chromosome& chromosome)
    : params(chromosome.params) {}

template <typename T, std::size_t N>
Chromosome<T, N>::Chromosome(Chromosome&& chromosome)
    : params(std::move(chromosome.params)) {}

template <typename T, std::size_t N>
Chromosome<T, N>::~Chromosome() {}

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const Chromosome<T, N>& chromosome) {
  std::string seperator = "";
  os << "<";

  for (const auto& param : chromosome.params) {
    std::cout << seperator << param;
    seperator = ", ";
  }

  os << ">";

  return os;
}

}  // namespace ga

#endif  // GA_CHROMOSOME_H_