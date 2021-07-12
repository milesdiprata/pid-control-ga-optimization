#ifndef GA_PROCEDURE_H_
#define GA_PROCEDURE_H_

#include <cstddef>

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

  Procedure();
  Procedure(const Args& args);
  ~Procedure();

 private:
  Args args_;
};

template <typename T, std::size_t N>
Procedure<T, N>::Procedure() {}

template <typename T, std::size_t N>
Procedure<T, N>::Procedure(const Args& args) : args_(args) {}

template <typename T, std::size_t N>
Procedure<T, N>::~Procedure() {}

}  // namespace ga

#endif  // GA_PROCEDURE_H_
