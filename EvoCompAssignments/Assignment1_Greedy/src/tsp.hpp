#pragma once

#include <string>
#include <vector>

struct Instance {
    std::string name;
    int n;
    int k;
    std::vector<int> xs;
    std::vector<int> ys;
    std::vector<int> costs;
    std::vector<std::vector<int>> dist;
};

Instance load_instance(const std::string& path);
long long tour_objective(const Instance& inst, const std::vector<int>& tour);
bool check_solution(const Instance& inst, const std::vector<int>& tour, long long expected);
