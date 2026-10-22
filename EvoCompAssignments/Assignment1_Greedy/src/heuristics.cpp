#include "heuristics.hpp"
#include <algorithm>
#include <numeric>

std::vector<int> random_solution(const Instance& inst, std::mt19937& rng) {
    std::vector<int> nodes(inst.n);
    std::iota(nodes.begin(), nodes.end(), 0);
    std::shuffle(nodes.begin(), nodes.end(), rng);
    nodes.resize(inst.k);
    return nodes;
}

std::vector<int> nearest_neighbor_end(const Instance& inst, int start) {
    std::vector<int> path;
    std::vector<int> used(inst.n, 0);
    path.push_back(start);
    used[start] = 1;

    while ((int)path.size() < inst.k) {
        int last = path.back();
        int best_node = -1;
        int best_val = 1000000000;
        for (int u = 0; u < inst.n; u++) {
            if (used[u]) continue;
            int val = inst.dist[last][u] + inst.costs[u];
            if (val < best_val) {
                best_val = val;
                best_node = u;
            }
        }
        path.push_back(best_node);
        used[best_node] = 1;
    }
    return path;
}

// How much the open-path objective grows if we insert node u at position pos.
int insertion_cost(const Instance& inst, const std::vector<int>& path, int u, int pos) {
    int extra = inst.costs[u];
    int m = (int)path.size();
    if (m == 0) return extra;
    if (pos == 0) return inst.dist[u][path[0]] + extra;
    if (pos == m) return inst.dist[path[m - 1]][u] + extra;
    int a = path[pos - 1];
    int b = path[pos];
    return inst.dist[a][u] + inst.dist[u][b] - inst.dist[a][b] + extra;
}

std::vector<int> nearest_neighbor_anywhere(const Instance& inst, int start) {
    std::vector<int> path;
    std::vector<int> used(inst.n, 0);
    path.push_back(start);
    used[start] = 1;

    while ((int)path.size() < inst.k) {
        int best_node = -1;
        int best_pos = -1;
        int best_val = 1000000000;
        for (int u = 0; u < inst.n; u++) {
            if (used[u]) continue;
            for (int pos = 0; pos <= (int)path.size(); pos++) {
                int val = insertion_cost(inst, path, u, pos);
                if (val < best_val) {
                    best_val = val;
                    best_node = u;
                    best_pos = pos;
                }
            }
        }
        path.insert(path.begin() + best_pos, best_node);
        used[best_node] = 1;
    }
    return path;
}

std::vector<int> greedy_cycle(const Instance& inst, int start) {
    std::vector<int> path;
    std::vector<int> used(inst.n, 0);
    path.push_back(start);
    used[start] = 1;

    // Second node: cheapest neighbour of the start (distance + node cost).
    int best_u = -1;
    int best_val = 1000000000;
    for (int u = 0; u < inst.n; u++) {
        if (u == start) continue;
        int val = inst.dist[start][u] + inst.costs[u];
        if (val < best_val) {
            best_val = val;
            best_u = u;
        }
    }
    path.push_back(best_u);
    used[best_u] = 1;

    while ((int)path.size() < inst.k) {
        int best_node = -1;
        int best_pos = -1;
        best_val = 1000000000;
        for (int u = 0; u < inst.n; u++) {
            if (used[u]) continue;
            for (int pos = 0; pos <= (int)path.size(); pos++) {
                int val = insertion_cost(inst, path, u, pos);
                if (val < best_val) {
                    best_val = val;
                    best_node = u;
                    best_pos = pos;
                }
            }
        }
        path.insert(path.begin() + best_pos, best_node);
        used[best_node] = 1;
    }
    return path;
}
