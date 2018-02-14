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

#include <PuzzleSolver.h>
#include <tuple>

std::tuple<float, uint, bool, pairs> PuzzleSolver::solvePuzzle() {
    auto operatorFunctor = [&](std::initializer_list<Operator> funcs) {
        std::accumulate(
                funcs.begin(),
                funcs.end(),
                std::ref(population),
                [&](population_ptr &acc, const auto &func) {
                    return std::ref(func(acc, conf));
                });
    };

    std::function<void()> functor = [&]() {
        operatorFunctor({std::bind(&PuzzleSolver::naturalSelection, this, std::placeholders::_1, std::placeholders::_2),
                         std::bind(&PuzzleSolver::crossover, this, std::placeholders::_1, std::placeholders::_2),
                         std::bind(&PuzzleSolver::mutation, this, std::placeholders::_1, std::placeholders::_2)});
    };

    auto stopCriteria = 0;
    auto done = [&] {
        const auto &it = min(population, conf);
        return objectiveFunction(*it, conf) <= stopCriteria;
    };

    bool isDone = true;
    if (!done()) {
        functor();
        isDone = false;
    }

    const auto &it = min(population, conf);

    return std::make_tuple<float, uint, bool, pairs>(
            objectiveFunction(*it, conf),
            (uint) ++iterations,
            std::move(isDone),
            chromosome2Pairs(it));
}

std::vector<chromosome_ptr>::iterator PuzzleSolver::min(population_ptr &population, const configuration_ptr &conf) {
    return std::min_element(population->begin(), population->end(), [&](const auto &ch1, const auto &ch2) {
        return objectiveFunction(ch1, conf) < objectiveFunction(ch2, conf);
    });
}

pairs PuzzleSolver::chromosome2Pairs(const std::vector<chromosome_ptr>::iterator &it) {
    return {};
}

PuzzleSolver::PuzzleSolver() {
    //TODO load conf.
    conf->emplace("naturalselection", "0.5f");
    conf->emplace("mutationrate", "0.4f");
    conf->emplace("side", "4");
    conf->emplace("size", std::to_string(30));
}

int PuzzleSolver::pair2Int(const std::unique_ptr<std::pair<int, int>> &pair, int side) {
    return pair->first * side + pair->second;
}

population_ptr &PuzzleSolver::naturalSelection(population_ptr &population, const configuration_ptr &conf) {
    auto naturalSelection = std::stof(conf->at("naturalselection"));

    std::sort(population->begin(), population->end(), [&](const auto &ch1, const auto &ch2) {
        return objectiveFunction(ch1, conf) < objectiveFunction(ch2, conf);
    });

    auto cut = static_cast<int>(population->size() * naturalSelection);
    auto offset = population->size() - cut;

    population->erase(population->begin() + offset, population->end());

    return population;
}

int PuzzleSolver::objectiveFunction(const chromosome_ptr &ch, const configuration_ptr &conf) {
    int side = std::stoi(conf->at("side"));
    auto i = 0;

    auto value = std::accumulate(ch->begin(), ch->end(), 0, [&](const auto &a, const auto &it) {
        return std::abs(a + (i++) - pair2Int(it, side));
    });

    for (auto j = 0u; j < ch->size() - 1; j++) {
        auto v = std::any_of(ch->begin() + j + 1, ch->end(), [&](const auto &it) {
            return (*it) == (*(*(ch->begin() + j)));
        });

        if (v) {
            value = std::numeric_limits<int>::max();
            break;
        }
    }

    return value;
}

population_ptr &PuzzleSolver::crossover(population_ptr &population, const configuration_ptr &conf) {
    auto size = std::stoi(conf->at("size"));
    auto currentSize = population->size();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis1(0, static_cast<int>(population->size() - 1));

    for (auto i = 0u; i < size - currentSize; ++i) {
        const auto &parent1 = (*population)[dis1(gen)];
        const auto &parent2 = (*population)[dis1(gen)];
        std::uniform_int_distribution<> dis2(0, static_cast<int>(parent1->size() - 1));
        auto pivot = dis2(gen);

        auto offspring1 = new Chromosome();
        for (auto it = parent1->begin(); it != parent1->begin() + pivot; ++it) {
            offspring1->emplace_back(std::make_unique<std::pair<int, int>>((*it)->first, (*it)->second));
        }
        for (auto it = parent2->begin() + pivot; it != parent2->end(); ++it) {
            offspring1->emplace_back(std::make_unique<std::pair<int, int>>((*it)->first, (*it)->second));
        }

        population->emplace_back(offspring1);
    }

    return population;
}

population_ptr &PuzzleSolver::mutation(population_ptr &population, const configuration_ptr &conf) {
    std::random_device rd;
    std::mt19937 gen(rd());
    auto mutationRate = std::stof(conf->at("mutationrate"));
    auto amount = static_cast<int>(population->size() * mutationRate);
    std::uniform_int_distribution<> dis1(0, static_cast<int>(population->size() - 1));
    std::uniform_int_distribution<> dis2(0, static_cast<int>((*population)[0]->size() - 1));

    for (auto i = 0; i < amount; ++i) {
        auto &parent = (*population)[dis1(gen)];

        auto p1 = dis2(gen);
        auto p2 = dis2(gen);

        std::iter_swap(parent->begin() + p1, parent->begin() + p2);
    }

    return population;
}
