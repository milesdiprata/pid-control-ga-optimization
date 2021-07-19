#ifndef GA_PROCEDURE_H_
#define GA_PROCEDURE_H_

#include <algorithm>
#include <cstddef>
#include <random>
#include <vector>

#include "ga/chromosome.h"

namespace ga {

template <typename T, std::size_t N>
class Procedure {
 public:
  static constexpr const double kAlpha = 0.5;
  static constexpr const std::size_t kNumSurvivors = 2;

  struct Args {
    static constexpr const std::size_t kDefaultPopulationSize = 50;
    static constexpr const std::size_t kDefaultNumGenerations = 150;

    static constexpr const double kDefaultCrossoverPr = 0.6;
    static constexpr const double kDefaultMutationPr = 0.25;

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

  constexpr Procedure(
      const Args& args = Args(),
      const std::vector<typename Gene<T>::Bounds>& constraints = {})
      : args_(args), constraints_(constraints), mt_(std::random_device{}()) {}

  virtual constexpr ~Procedure() = default;

  constexpr const Args& args() const { return args_; }
  constexpr Args& args() { return args_; }

  constexpr const Chromosome<T, N> Start() {
    auto generation = InitGeneration();
    Randomize(generation);
    Evaluate(generation);
    for (const auto& c : generation)
      std::cout << c << " " << c.fitness() << "\n";

    for (std::size_t k = 0; k < args_.num_generations; ++k) {
      auto new_generation = InitGeneration();

      CopySurvivors(generation, new_generation);

      Crossover(SelectParents(generation), new_generation);

      Mutate(new_generation);

      Evaluate(new_generation);

      generation = new_generation;

      return generation.front();
    }

    return generation.front();
  }

 protected:
  virtual constexpr const double Fitness(
      const Chromosome<T, N>& chromosome) = 0;

 private:
  struct Compare {
    constexpr const bool operator()(const Chromosome<T, N>& c1,
                                    const Chromosome<T, N>& c2) const {
      return c1.fitness() < c2.fitness();
    }
  };

  constexpr const bool Terminate(const std::size_t num_generations) const {
    // Termination criteria could be:
    // * Specified number of generations or fitness evaluations
    // * Minimum (solution) threshold reached
    // * No improvement in best individual for a specified number of
    // generations
    // * Memory/time constraints

    return num_generations > args_.num_generations;
  }

  constexpr const std::vector<Chromosome<T, N>> InitGeneration() const {
    auto generation = std::vector<Chromosome<T, N>>(args_.population_size);
    for (auto& chromosome : generation) {
      chromosome = Chromosome<T, N>(constraints_);
    }

    return generation;
  }

  constexpr void Randomize(std::vector<Chromosome<T, N>>& generation) {
    for (auto& chromosome : generation) {
      chromosome.randomize();
    }
  }

  constexpr void Evaluate(std::vector<Chromosome<T, N>>& generation) {
    for (auto& chromosome : generation) {
      chromosome.fitness() = Fitness(chromosome);
    }

    std::sort(generation.begin(), generation.end(), Compare());
  }

  constexpr void CopySurvivors(const std::vector<Chromosome<T, N>>& generation,
                               std::vector<Chromosome<T, N>>& new_generation) {
    for (std::size_t i = 0; i < kNumSurvivors; ++i) {
      new_generation.push_back(generation[i]);
    }
  }

  const std::vector<Chromosome<T, N>*> SelectParents(
      std::vector<Chromosome<T, N>>& generation) {
    double sum_of_fitness = 0;
    for (const auto& chromosome : generation) {
      sum_of_fitness += chromosome.fitness();
    }

    double prev_selection_pr = 0;
    for (auto& chromosome : generation) {
      double selection_pr =
          prev_selection_pr + (chromosome.fitness() / sum_of_fitness);

      chromosome.selection_pr() = selection_pr;
      prev_selection_pr = selection_pr;
    }

    auto dis = std::uniform_real_distribution<>();
    auto parents = std::vector<Chromosome<T, N>*>();

    std::size_t num_chromosomes = generation.size();
    std::size_t num_spins = num_chromosomes - (num_chromosomes % 2);

    for (std::size_t i = 0; i < num_spins; ++i) {
      double rand = dis(mt_);
      for (auto& chromosome : generation) {
        if (rand < chromosome.selection_pr()) {
          parents.push_back(&chromosome);
          break;
        }
      }
    }

    return parents;
  }

  void Crossover(const std::vector<Chromosome<T, N>*>& parents,
                 std::vector<Chromosome<T, N>>& new_generation) {
    auto dis = std::uniform_real_distribution<>();

    auto offspring = std::vector<Chromosome<T, N>>();
    for (std::size_t i = 0, size = parents.size() - 1; i < size; i += 2) {
      if (dis(mt_) < args_.crossover_pr) {
        auto& first_parent = *parents[i];
        auto& second_parent = *parents[i + 1];

        auto first_child = Chromosome<T, N>();
        auto second_child = Chromosome<T, N>();

        for (std::size_t j = 0; j < N; ++j) {
          first_child[j] = first_parent[j];
          first_child[j].value() = (kAlpha * first_parent[j].value()) +
                                   ((1.0 - kAlpha) * second_parent[j].value());

          second_child[j] = second_parent[j];
          second_child[j].value() = ((1.0 - kAlpha) * first_parent[j].value()) +
                                    (kAlpha * second_parent[j].value());
        }

        first_child.fitness() = Fitness(first_child);
        second_child.fitness() = Fitness(second_child);

        new_generation.push_back(first_child);
        new_generation.push_back(second_child);
      }
    }
  }

  void Mutate(std::vector<Chromosome<T, N>>& generation) {
    auto dis = std::uniform_real_distribution<>();
    auto mutation_dis = typename Gene<T>::uniform_distribution(0, 0.01);

    for (auto& chromosome : generation) {
      for (auto& gene : chromosome) {
        if (dis(mt_) < args_.mutation_pr) {
          gene.value() = gene.value() + mutation_dis(mt_);

          if (gene.bounds().has_value()) {
            if (gene.value() < gene.bounds()->lower) {
              gene.value() = gene.bounds()->lower;
            } else if (gene.value() > gene.bounds()->upper) {
              gene.value() = gene.bounds()->upper;
            }
          }
        }
      }
    }
  }

  // void SurvivorSelection(std::vector<Chromosome<T, N>>& offspring) {
  //   std::sort(offspring.begin(), offspring.end(), Compare());

  //   std::size_t num_survivors = std::min(kNumSurvivors, offspring.size());
  //   for (std::size_t i = 0; i < num_survivors; ++i) {
  //     chromosomes_.push_back(offspring[i]);
  //   }

  //   std::sort(chromosomes_.begin(), chromosomes_.end(), Compare());
  //   for (std::size_t i = 0; i < num_survivors; ++i) {
  //     chromosomes_.pop_back();
  //   }

  //   for (const auto& c : chromosomes_) {
  //     std::cout << c << " " << c.fitness() << std::endl;
  //   }
  // }

  Args args_;
  std::vector<typename Gene<T>::Bounds> constraints_;
  // std::vector<Chromosome<T, N>> chromosomes_;
  // std::size_t num_generations_;
  std::mt19937_64 mt_;
};

}  // namespace ga

#endif  // GA_PROCEDURE_H_
