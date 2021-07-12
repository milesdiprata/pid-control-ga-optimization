#ifndef GA_GENE_H_
#define GA_GENE_H_

#include <iostream>
#include <optional>
#include <utility>

namespace ga {

template <typename T>
struct Gene;

template <typename T>
std::ostream& operator<<(std::ostream& os, const Gene<T>& gene);

template <typename T>
struct Gene {
  struct Bounds {
    Bounds(const T& lower, const T& upper) : lower(lower), upper(upper) {}
    Bounds(const Bounds& bounds) : lower(bounds.lower), upper(bounds.upper) {}
    Bounds(Bounds&& bounds)
        : lower(std::move(bounds.lower)), upper(std::move(bounds.upper)) {}
    ~Bounds() = default;

    T lower;
    T upper;
  };

  Gene();
  Gene(const T& value);
  Gene(const T& value, const T& lower_bound, const T& upper_bound);
  Gene(const T& value, const Bounds& bounds);
  Gene(const Gene& gene);
  Gene(Gene&& gene);
  ~Gene();

  T value;
  std::optional<Bounds> bounds;
};

template <typename T>
Gene<T>::Gene() {}

template <typename T>
Gene<T>::Gene(const T& value) : value(value), bounds(std::nullopt) {}

template <typename T>
Gene<T>::Gene(const T& value, const T& lower_bound, const T& upper_bound)
    : value(value), bounds(lower_bound, upper_bound) {}

template <typename T>
Gene<T>::Gene(const T& value, const Bounds& bounds)
    : value(value), bounds(bounds) {}

template <typename T>
Gene<T>::Gene(const Gene& gene) : value(gene.value), bounds(gene.bounds) {}

template <typename T>
Gene<T>::Gene(Gene&& gene)
    : value(std::move(gene.value)), bounds(std::move(gene.bounds)) {}

template <typename T>
Gene<T>::~Gene() {}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Gene<T>& gene) {
  return os << gene.value;
}

}  // namespace ga

#endif  // GA_GENE_H_