#ifndef GA_GENE_H_
#define GA_GENE_H_

#include <iostream>
#include <optional>
#include <random>
#include <type_traits>

namespace ga {

template <typename T>
class Gene;

template <typename T>
std::ostream& operator<<(std::ostream& os, const Gene<T>& gene);

template <typename T>
class Gene {
 public:
  using uniform_distribution = typename std::conditional<
      std::is_floating_point<T>::value, std::uniform_real_distribution<T>,
      typename std::conditional<std::is_integral<T>::value,
                                std::uniform_int_distribution<T>,
                                void>::type>::type;

  constexpr Gene(const T& value = T()) : value_(value), bounds_(std::nullopt) {}

  constexpr Gene(const T& lower_bound, const T& upper_bound)
      : bounds_(Bounds(lower_bound, upper_bound)) {}

  constexpr Gene(const T& value, const T& lower_bound, const T& upper_bound)
      : value_(value), bounds_(Bounds(lower_bound, upper_bound)) {}

  constexpr Gene(const Gene& gene)
      : value_(gene.value_), bounds_(gene.bounds_) {}

  constexpr ~Gene() = default;

  constexpr const T& value() const { return value_; }
  inline T& value() { return value_; }

  constexpr const T& lower_bound() const { return bounds_.lower_bound; }
  inline T& lower_bound() { return bounds_.lower_bound; }

  constexpr const T& upper_bound() const { return bounds_.upper_bound; }
  inline T& upper_bound() { return bounds_.upper_bound; }

  constexpr void Reset() { value_ = T(); }
  void Randomize();

  friend std::ostream& operator<<<>(std::ostream& os, const Gene& gene);

 private:
  struct Bounds {
    constexpr Bounds(const T& lower, const T& upper)
        : lower(lower), upper(upper) {}

    constexpr Bounds(const Bounds& bounds)
        : lower(bounds.lower), upper(bounds.upper) {}

    constexpr ~Bounds() = default;

    T lower;
    T upper;
  };

  T value_;
  std::optional<Bounds> bounds_;
};

template <typename T>
void Gene<T>::Randomize() {
  static auto mt = std::mt19937_64(std::random_device{}());
  auto dis = bounds_ ? uniform_distribution(bounds_->lower, bounds_->upper)
                     : uniform_distribution();
  value_ = dis(mt);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Gene<T>& gene) {
  return os << gene.value_;
}

}  // namespace ga

#endif  // GA_GENE_H_
