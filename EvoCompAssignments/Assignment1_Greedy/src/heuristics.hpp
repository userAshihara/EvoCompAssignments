#pragma once

#include "tsp.hpp"

#include <random>
#include <vector>

std::vector<int> random_solution(const Instance& inst, std::mt19937& rng);
std::vector<int> nearest_neighbor_end(const Instance& inst, int start);
std::vector<int> nearest_neighbor_anywhere(const Instance& inst, int start);
std::vector<int> greedy_cycle(const Instance& inst, int start);
