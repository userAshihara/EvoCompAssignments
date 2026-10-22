#include "tsp.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

Instance load_instance(const std::string& path) {
    Instance inst;
    inst.name = path;

    // Keep only the file name without folders or ".csv".
    int slash = -1;
    for (int i = 0; i < (int)path.size(); i++) {
        if (path[i] == '/' || path[i] == '\\') slash = i;
    }
    inst.name = path.substr(slash + 1);
    if (inst.name.size() >= 4) {
        inst.name = inst.name.substr(0, inst.name.size() - 4);
    }

    std::ifstream in(path);
    if (!in) {
        std::cerr << "Cannot open " << path << "\n";
        inst.n = 0;
        inst.k = 0;
        return inst;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        for (int i = 0; i < (int)line.size(); i++) {
            if (line[i] == ';') line[i] = ' ';
        }
        std::istringstream ss(line);
        int x, y, c;
        ss >> x >> y >> c;
        inst.xs.push_back(x);
        inst.ys.push_back(y);
        inst.costs.push_back(c);
    }

    inst.n = (int)inst.costs.size();
    inst.k = (inst.n + 1) / 2;

    inst.dist.assign(inst.n, std::vector<int>(inst.n, 0));
    for (int i = 0; i < inst.n; i++) {
        for (int j = i + 1; j < inst.n; j++) {
            double dx = inst.xs[i] - inst.xs[j];
            double dy = inst.ys[i] - inst.ys[j];
            int d = (int)std::round(std::sqrt(dx * dx + dy * dy));
            inst.dist[i][j] = d;
            inst.dist[j][i] = d;
        }
    }
    return inst;
}

long long tour_objective(const Instance& inst, const std::vector<int>& tour) {
    if (tour.empty()) return 0;
    long long total = 0;
    int m = (int)tour.size();
    for (int i = 0; i < m; i++) {
        total += inst.costs[tour[i]];
        total += inst.dist[tour[i]][tour[(i + 1) % m]];
    }
    return total;
}

bool check_solution(const Instance& inst, const std::vector<int>& tour, long long expected) {
    if ((int)tour.size() != inst.k) {
        std::cerr << "Wrong tour length\n";
        return false;
    }
    for (int i = 0; i < (int)tour.size(); i++) {
        if (tour[i] < 0 || tour[i] >= inst.n) {
            std::cerr << "Node out of range\n";
            return false;
        }
        for (int j = i + 1; j < (int)tour.size(); j++) {
            if (tour[i] == tour[j]) {
                std::cerr << "Duplicate node in tour\n";
                return false;
            }
        }
    }
    long long computed = tour_objective(inst, tour);
    if (computed != expected) {
        std::cerr << "Objective mismatch\n";
        return false;
    }
    return true;
}
