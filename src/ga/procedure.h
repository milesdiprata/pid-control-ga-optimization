#ifndef GA_PROCEDURE_H_
#define GA_PROCEDURE_H_

#include <algorithm>
#include <cstddef>
#include <iterator>
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
    auto generation = RandomGeneration();
    EvaluateFitness(generation);
    std::sort(generation.begin(), generation.end(), CompareFitness());

    std::size_t num_generations = 0;
    while (!Terminate(num_generations)) {
      auto new_generation = std::vector<Chromosome<T, N>>(
          generation.begin(), generation.begin() + kNumSurvivors);

      auto offspring = WholeArithmeticCrossover(SelectParentsFPS(generation));

      UniformMutation(offspring);

      new_generation.insert(new_generation.end(),
                            std::make_move_iterator(offspring.begin()),
                            std::make_move_iterator(offspring.end()));

      EvaluateFitness(new_generation);

      generation = new_generation;
      ++num_generations;
    }

    return *std::max_element(generation.begin(), generation.end(),
                             CompareFitness());
  }

 protected:
  virtual constexpr const double Fitness(
      const Chromosome<T, N>& chromosome) = 0;

 private:
  struct Parent {
    constexpr Parent(const Chromosome<T, N>* first = nullptr,
                     const Chromosome<T, N>* second = nullptr)
        : first(first), second(second) {}
    constexpr ~Parent() = default;

    const Chromosome<T, N>* first;
    const Chromosome<T, N>* second;
  };

  struct CompareFitness {
    constexpr const bool operator()(const Chromosome<T, N>& c1,
                                    const Chromosome<T, N>& c2) const {
      return c1.fitness() < c2.fitness();
    }
  };

  struct CompareSelectionPr {
    constexpr const bool operator()(const Chromosome<T, N>& c1,
                                    const Chromosome<T, N>& c2) const {
      return c1.selection_pr() < c2.selection_pr();
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

  constexpr const std::vector<Chromosome<T, N>> RandomGeneration() const {
    auto generation = std::vector<Chromosome<T, N>>(args_.population_size);
    for (auto& chromosome : generation) {
      chromosome = Chromosome<T, N>(constraints_);
      chromosome.randomize();
    }

    return generation;
  }

  constexpr void EvaluateFitness(std::vector<Chromosome<T, N>>& generation) {
    for (auto& chromosome : generation) {
      chromosome.fitness() = Fitness(chromosome);
    }
  }

  const std::vector<Parent> SelectParentsFPS(
      std::vector<Chromosome<T, N>>& generation) {
    double sum_of_fitness = 0.0;
    for (const auto& chromosome : generation) {
      sum_of_fitness += chromosome.fitness();
    }

    double prev_pr = 0.0;
    for (auto& chromosome : generation) {
      double selection_pr = prev_pr + (chromosome.fitness() / sum_of_fitness);

      chromosome.selection_pr() = selection_pr;
      prev_pr = selection_pr;
    }

    std::sort(generation.begin(), generation.end(), CompareSelectionPr());

    auto parents = std::vector<Parent>((generation.size() - kNumSurvivors) / 2);
    for (auto& parent : parents) {
      parent = SelectParent(generation);
    }

    return parents;
  }

  const Parent SelectParent(const std::vector<Chromosome<T, N>>& generation) {
    auto parent = Parent();
    auto dis = std::uniform_real_distribution<>();

    for (const auto& chromosome : generation) {
      if (dis(mt_) < chromosome.selection_pr()) {
        if (!parent.first) {
          parent.first = &chromosome;
        } else {
          parent.second = &chromosome;
          break;
        }
      }
    }

    return parent;
  }

  const std::vector<Chromosome<T, N>> WholeArithmeticCrossover(
      const std::vector<Parent>& parents) {
    auto offspring = std::vector<Chromosome<T, N>>();
    auto dis = std::uniform_real_distribution<>();

    for (const auto& parent : parents) {
      auto& first_parent = *parent.first;
      auto& second_parent = *parent.second;

      if (dis(mt_) < args_.crossover_pr) {
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

        offspring.push_back(first_child);
        offspring.push_back(second_child);
      } else {
        offspring.push_back(first_parent);
        offspring.push_back(second_parent);
      }
    }

    return offspring;
  }

  void UniformMutation(std::vector<Chromosome<T, N>>& offspring) {
    auto dis = std::uniform_real_distribution<>();

    for (auto& chromosome : offspring) {
      for (auto& gene : chromosome) {
        if (dis(mt_) < args_.mutation_pr) {
          gene.randomize();
        }
      }
    }
  }

  Args args_;
  std::vector<typename Gene<T>::Bounds> constraints_;
  std::mt19937_64 mt_;
};

}  // namespace ga

#endif  // GA_PROCEDURE_H_
