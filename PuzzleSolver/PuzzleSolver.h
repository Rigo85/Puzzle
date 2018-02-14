//
// Author Rigoberto Leander Salgado Reyes <rlsalgado2006@gmail.com>
//
// Copyright 2018 by Rigoberto Leander Salgado Reyes.
//
// This program is licensed to you under the terms of version 3 of the
// GNU Affero General Public License. This program is distributed WITHOUT
// ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THOSE OF NON-INFRINGEMENT,
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Please refer to the
// AGPL (http:www.gnu.org/licenses/agpl-3.0.txt) for more details.
//

#pragma once

#include <iostream>
#include <random>
#include <map>
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

using pairs = std::vector<std::pair<uint, uint>>;

using pair_ptr = std::unique_ptr<std::pair<int, int>>;
using Chromosome = std::vector<pair_ptr>;
using chromosome_ptr = std::unique_ptr<Chromosome>;
using Population = std::vector<chromosome_ptr>;
using Configuration = std::map<std::string, std::string>;
using population_ptr = std::unique_ptr<Population>;
using configuration_ptr = std::unique_ptr<Configuration>;
using Operator = std::function<population_ptr &(population_ptr &, const configuration_ptr &)>;

class PuzzleSolver {
private:
    population_ptr population = std::make_unique<Population>();
    configuration_ptr conf = std::make_unique<Configuration>();
    int iterations = 0;

public:
    PuzzleSolver();

    virtual ~PuzzleSolver() = default;

    std::tuple<float, uint, bool, pairs> solvePuzzle();

//private:
    population_ptr &naturalSelection(population_ptr &, const configuration_ptr &);

    population_ptr &crossover(population_ptr &, const configuration_ptr &);

    population_ptr &mutation(population_ptr &, const configuration_ptr &);

//    void showPopulation(const population_ptr &, const configuration_ptr &);

    int objectiveFunction(const chromosome_ptr &, const configuration_ptr &);

//    void showChromosome(const chromosome_ptr &, const configuration_ptr &);

    int pair2Int(const pair_ptr &, int);

    std::vector<chromosome_ptr>::iterator min(population_ptr &population, const configuration_ptr &conf);

    pairs chromosome2Pairs(const std::vector<chromosome_ptr>::iterator &it);
};
