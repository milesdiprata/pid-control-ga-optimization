#ifndef GA_CHROMOSOME_H_
#define GA_CHROMOSOME_H_

#include <array>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

#include "ga/gene.h"

namespace ga {

template <typename T, std::size_t N>
class Chromosome;

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const Chromosome<T, N>& chromosome);

template <typename T, std::size_t N>
class Chromosome {
 public:
  constexpr Chromosome() = default;

  constexpr Chromosome(const Chromosome& chromosome)
      : genes_(chromosome.genes_) {}

  constexpr ~Chromosome() = default;

  inline const Gene<T>& operator[](const std::size_t i) const {
    return genes_[i];
  }
  inline Gene<T>& operator[](const std::size_t i) { return genes_[i]; }

  inline constexpr Gene<T>& front() const { return genes_.front(); }
  inline Gene<T>& front() { return genes_.front(); }

  inline constexpr Gene<T>* begin() const { return &genes_.begin(); }
  inline Gene<T>* begin() { return &genes_.begin(); }

  inline constexpr Gene<T>& back() const { return genes_.back(); }
  inline Gene<T>& back() { return genes_.back(); }

  inline constexpr Gene<T>* end() const { return &genes_.end(); }
  inline Gene<T>* end() { return &genes_.end(); }

  constexpr void Reset();
  void Randomize();

  friend std::ostream& operator<<<>(std::ostream& os,
                                    const Chromosome<T, N>& chromosome);

 private:
  std::array<Gene<T>, N> genes_;
};

template <typename T, std::size_t N>
inline constexpr void Chromosome<T, N>::Reset() {
  for (auto& gene : genes_) {
    gene.Reset();
  }
}

template <typename T, std::size_t N>
void Chromosome<T, N>::Randomize() {
  for (auto& gene : genes_) {
    gene.Randomize();
  }
}

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const Chromosome<T, N>& chromosome) {
  std::string seperator = "";
  os << "<";

  for (const auto& gene : chromosome.genes_) {
    std::cout << seperator << gene;
    seperator = ", ";
  }

  os << ">";

  return os;
}

}  // namespace ga

#endif  // GA_CHROMOSOME_H_