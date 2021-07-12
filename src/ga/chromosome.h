#ifndef GA_CHROMOSOME_H_
#define GA_CHROMOSOME_H_

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "ga/gene.h"

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

  std::vector<Gene<T>> genes;
};

template <typename T, std::size_t N>
Chromosome<T, N>::Chromosome() : genes(N) {}

template <typename T, std::size_t N>
Chromosome<T, N>::Chromosome(const Chromosome& chromosome)
    : genes(chromosome.genes) {}

template <typename T, std::size_t N>
Chromosome<T, N>::Chromosome(Chromosome&& chromosome)
    : genes(std::move(chromosome.genes)) {}

template <typename T, std::size_t N>
Chromosome<T, N>::~Chromosome() {}

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const Chromosome<T, N>& chromosome) {
  std::string seperator = "";
  os << "<";

  for (const auto& gene : chromosome.genes) {
    std::cout << seperator << gene;
    seperator = ", ";
  }

  os << ">";

  return os;
}

}  // namespace ga

#endif  // GA_CHROMOSOME_H_