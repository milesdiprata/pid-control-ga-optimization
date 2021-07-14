#ifndef GA_PROCEDURE_H_
#define GA_PROCEDURE_H_

#include <algorithm>
#include <cstddef>
#include <memory>
#include <random>
#include <span>
#include <unordered_map>
#include <vector>

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

    constexpr ~Args() = default;

    std::size_t population_size;
    std::size_t num_generations;
    double crossover_pr;
    double mutation_pr;
  };

  Procedure(const Args& args = Args());
  Procedure(const Chromosome<T, N>& chromosome_template);
  Procedure(const Args& args, const Chromosome<T, N>& chromosome_template);
  ~Procedure();

  inline constexpr Args& args() const { return args_; }
  inline Args& args() { return args_; }

  void Start();

 private:
  void InitChromosomes(const Chromosome<T, N>& chromosome_template);

  const bool Terminate() const;
  const std::vector<Chromosome<T, N>*> SelectParents();
  const std::vector<Chromosome<T, N>> Crossover(
      const std::vector<Chromosome<T, N>*>& parents) const;
  void Mutate(Chromosome<T, N>& chromosome);
  void UpdatePopulation();

  inline virtual const double Fitness(
      const Chromosome<T, N>& chromosome) const {
    return 1;
  }

  Args args_;
  std::unique_ptr<Chromosome<T, N>[]> chromosomes_;
  std::size_t num_generations_;
  std::mt19937_64 mt_;
};

template <typename T, std::size_t N>
Procedure<T, N>::Procedure(const Args& args)
    : args_(args),
      chromosomes_(std::make_unique<Chromosome<T, N>[]>(args_.population_size)),
      num_generations_(0),
      mt_(std::random_device{}()) {}

template <typename T, std::size_t N>
Procedure<T, N>::Procedure(const Chromosome<T, N>& chromosome_template)
    : Procedure() {
  InitChromosomes(chromosome_template);
}

template <typename T, std::size_t N>
Procedure<T, N>::Procedure(const Args& args,
                           const Chromosome<T, N>& chromosome_template)
    : Procedure(args) {
  InitChromosomes(chromosome_template);
}

template <typename T, std::size_t N>
Procedure<T, N>::~Procedure() {}

template <typename T, std::size_t N>
void Procedure<T, N>::Start() {
  num_generations_ = 0;

  auto chromosomes = std::span(chromosomes_.get(), args_.population_size);
  for (auto& chromosome : chromosomes) {
    chromosome.Randomize();
    chromosome.fitness() = Fitness(chromosome);
  }

  while (!Terminate()) {
    auto offspring = Crossover(SelectParents());
    return;
    for (auto& chromosome : offspring) {
      Mutate(chromosome);
    }

    UpdatePopulation();
  }
}

template <typename T, std::size_t N>
void Procedure<T, N>::InitChromosomes(
    const Chromosome<T, N>& chromosome_template) {
  auto chromosomes = std::span(chromosomes_.get(), args_.population_size);
  for (auto& chromosome : chromosomes) {
    chromosome = chromosome_template;
  }
}

template <typename T, std::size_t N>
const bool Procedure<T, N>::Terminate() const {
  // Termination criteria could be:
  // * Specified number of generations or fitness evaluations
  // * Minimum (solution) threshold reached
  // * No improvement in best individual for a specified number of generations
  // * Memory/time constraints

  return num_generations_ > args_.num_generations;
}

template <typename T, std::size_t N>
const std::vector<Chromosome<T, N>*> Procedure<T, N>::SelectParents() {
  auto chromosomes = std::span(chromosomes_.get(), args_.population_size);

  double sum_of_fitness = 0;
  for (const auto& chromosome : chromosomes) {
    sum_of_fitness += chromosome.fitness();
  }

  // double prev_selection_pr = 0;
  // for (auto& chromosome : chromosomes) {
  //   double selection_pr =
  //       prev_selection_pr + (chromosome.fitness() / sum_of_fitness);

  //   chromosome.selection_pr() = selection_pr;
  //   prev_selection_pr = selection_pr;
  // }

  for (std::size_t i = 0; i < chromosomes.size(); ++i)
    chromosomes[i].selection_pr() += (double)i / chromosomes.size();

  std::sort(
      chromosomes.begin(), chromosomes.end(),
      [](const Chromosome<T, N>& c1, const Chromosome<T, N>& c2) -> const bool {
        return c1.fitness() < c2.fitness();
      });

  // for (const auto& c : chromosomes)
  //   std::cout << c << " " << c.selection_pr() << std::endl;

  auto dis = std::uniform_real_distribution<>();
  auto parents = std::vector<Chromosome<T, N>*>();

  std::size_t num_chromosomes = chromosomes.size();
  std::size_t num_spins = num_chromosomes - (num_chromosomes % 2);

  for (std::size_t i = 0; i < num_spins; ++i) {
    double rand = dis(mt_);
    for (auto& chromosome : chromosomes) {
      if (rand < chromosome.selection_pr()) {
        parents.push_back(&chromosome);
        break;
      }
    }
  }

  return parents;
}

template <typename T, std::size_t N>
const std::vector<Chromosome<T, N>> Procedure<T, N>::Crossover(
    const std::vector<Chromosome<T, N>*>& parents) const {
  auto offspring = std::vector<Chromosome<T, N>>();
  auto dis = std::uniform_real_distribution<>();

  for (const auto* const parent : parents) {
    if (dis(mt_) < args_.crossover_pr) {
    }
  }

  return offspring;
}

template <typename T, std::size_t N>
void Procedure<T, N>::Mutate(Chromosome<T, N>& chromosome) {
  auto dis = std::uniform_real_distribution<>();
  if (dis(mt_) < args_.mutation_pr) {
  }
}

template <typename T, std::size_t N>
void Procedure<T, N>::UpdatePopulation() {}

}  // namespace ga

#endif  // GA_PROCEDURE_H_
