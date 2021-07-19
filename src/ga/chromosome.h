#ifndef GA_CHROMOSOME_H_
#define GA_CHROMOSOME_H_

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "ga/gene.h"

namespace ga {

template <typename T, std::size_t N>
class Chromosome;

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const Chromosome<T, N>& chromosome);

template <typename T, std::size_t N>
class Chromosome {
 public:
  constexpr Chromosome()
      : genes_(std::vector<Gene<T>>(N)), fitness_(0.0), selection_pr_(0.0) {}

  constexpr Chromosome(const std::vector<typename Gene<T>::Bounds>& bounds)
      : Chromosome() {
    for (std::size_t i = 0, size = std::min(N, bounds.size()); i < size; ++i) {
      genes_[i] = Gene<T>(bounds[i]);
    }
  }

  constexpr Chromosome(const Chromosome& chromosome)
      : genes_(chromosome.genes_),
        fitness_(chromosome.fitness_),
        selection_pr_(chromosome.selection_pr_) {}

  virtual constexpr ~Chromosome() = default;

  constexpr const std::size_t size() const { return genes_.size(); }

  constexpr const Gene<T>& operator[](const std::size_t i) const {
    return genes_[i];
  }
  constexpr Gene<T>& operator[](const std::size_t i) { return genes_[i]; }

  constexpr const Gene<T>& front() const { return genes_.front(); }
  constexpr Gene<T>& front() { return genes_.front(); }

  constexpr const typename std::vector<Gene<T>>::iterator begin() const {
    return genes_.begin();
  }
  constexpr typename std::vector<Gene<T>>::iterator begin() {
    return genes_.begin();
  }

  constexpr const Gene<T>& back() const { return genes_.back(); }
  constexpr Gene<T>& back() { return genes_.back(); }

  constexpr const typename std::vector<Gene<T>>::iterator end() const {
    return genes_.end();
  }
  constexpr typename std::vector<Gene<T>>::iterator end() {
    return genes_.end();
  }

  constexpr const double fitness() const { return fitness_; }
  constexpr double& fitness() { return fitness_; }

  constexpr const double selection_pr() const { return selection_pr_; }
  constexpr double& selection_pr() { return selection_pr_; }

  constexpr void reset() {
    for (auto& gene : genes_) {
      gene.Reset();
    }

    fitness_ = 0.0;
    selection_pr_ = 0.0;
  }

  void randomize() {
    for (auto& gene : genes_) {
      gene.randomize();
    }

    fitness_ = 0;
    selection_pr_ = 0;
  }

  friend std::ostream& operator<<<>(std::ostream& os,
                                    const Chromosome<T, N>& chromosome);

 private:
  std::vector<Gene<T>> genes_;
  double fitness_;
  double selection_pr_;
};

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
