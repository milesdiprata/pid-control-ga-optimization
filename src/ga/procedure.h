#ifndef GA_PROCEDURE_H_
#define GA_PROCEDURE_H_

#include <array>
#include <cstddef>
#include <memory>

#include "ga/chromosome.h"

namespace ga {

template <typename T, std::size_t N>
class Procedure {
 public:
  struct Args {
    static constexpr std::size_t kDefaultPopulationSize = 50;
    static constexpr std::size_t kDefaultNumGenerations = 150;
    static constexpr double kDefaultCrossoverPr = 0.6;
    static constexpr double kDefaultMutationPr = 0.25;

    constexpr Args(const std::size_t population_size = kDefaultPopulationSize,
                   const std::size_t num_generations = kDefaultNumGenerations,
                   const double crossover_pr = kDefaultCrossoverPr,
                   const double mutation_pr = kDefaultMutationPr)
        : population_size(population_size),
          num_generations(num_generations),
          crossover_pr(crossover_pr),
          mutation_pr(mutation_pr) {}

    constexpr Args(const Args& args)
        : population_size(args.population_size),
          num_generations(args.num_generations),
          crossover_pr(args.crossover_pr),
          mutation_pr(args.mutation_pr) {}

    ~Args() = default;

    std::size_t population_size;
    std::size_t num_generations;
    double crossover_pr;
    double mutation_pr;
  };

  Procedure(const Args& args = Args(),
            const Chromosome<T, N>& chromosome_template = Chromosome<T, N>());
  ~Procedure();

  inline constexpr Args& args() const { return args_; }
  inline Args& args() { return args_; }

  void Start();

 private:
  struct ChromosomePair {
    static constexpr std::size_t kNumChromosomes = 2;

    ChromosomePair(const Chromosome<T, N>& first,
                   const Chromosome<T, N>& second)
        : ChromosomePair({first, second}) {}
    ~ChromosomePair() = default;

    inline const Chromosome<T, N>& first() const { return chromosomes.front(); }
    inline Chromosome<T, N>& first() { return chromosomes.front(); }

    inline const Chromosome<T, N>& second() const { return chromosomes.back(); }
    inline Chromosome<T, N>& second() { return chromosomes.back(); }

    inline const Chromosome<T, N>* begin() const { return chromosomes.begin(); }
    inline Chromosome<T, N>* begin() { return chromosomes.begin(); }

    inline const Chromosome<T, N>* end() const { return chromosomes.end(); }
    inline Chromosome<T, N>* end() { return chromosomes.end(); }

    std::array<Chromosome<T, N>, kNumChromosomes> chromosomes;
  };

  const bool Terminate() const;
  const ChromosomePair SelectParents() const;
  const ChromosomePair Crossover(const ChromosomePair& parents) const;
  void Mutate(Chromosome<T, N>& chromosome) const;
  void UpdatePopulation();

  static const double Fitness(const Chromosome<T, N>& chromosome);

  Args args_;
  std::unique_ptr<Chromosome<T, N>[]> chromosomes_;
  std::size_t num_generations_;
};

template <typename T, std::size_t N>
Procedure<T, N>::Procedure(const Args& args,
                           const Chromosome<T, N>& chromosome_template)
    : args_(args),
      chromosomes_(std::make_unique<Chromosome<T, N>[]>(args_.population_size)),
      num_generations_(0) {
  for (std::size_t i = 0; i < N; ++i) {
    chromosomes_[i] = chromosome_template;
  }
}

template <typename T, std::size_t N>
Procedure<T, N>::~Procedure() {}

template <typename T, std::size_t N>
void Procedure<T, N>::Start() {
  num_generations_ = 0;

  for (std::size_t i = 0; i < N; ++i) {
    chromosomes_[i].Randomize();
  }

  while (!Terminate()) {
    auto offspring = Crossover(SelectParents());
    for (auto& chromosome : offspring) {
      Mutate(chromosome);
    }

    Update();
  }
}

template <typename T, std::size_t N>
const bool Procedure<T, N>::Terminate() const {
  // Termination criteria could be:
  // * Specified number of generations or fitness evaluations
  // * Minimum (solution) threshold reached
  // * No improvement in best individual for a specified number of generations
  // * Memory/time constraints
  return true;
}

}  // namespace ga

#endif  // GA_PROCEDURE_H_
