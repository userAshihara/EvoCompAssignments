#include "heuristics.hpp"
#include "tsp.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int main() {
    std::string files[2] = {"../TSPA.csv", "../TSPB.csv"};
    std::string methods[4] = {
        "random",
        "nearest_neighbor_end",
        "nearest_neighbor_anywhere",
        "greedy_cycle"};

    std::ofstream out("results.txt");
    out << "method min max avg checker\n";

    for (int f = 0; f < 2; f++) {
        Instance inst = load_instance(files[f]);
        if (inst.n == 0) return 1;
        std::cout << "Instance " << inst.name << " (n=" << inst.n << ", K=" << inst.k << ")\n";

        std::mt19937 rng(42);

        for (int m = 0; m < 4; m++) {
            long long best = 0;
            long long worst = 0;
            long long sum = 0;
            std::vector<int> best_tour;
            bool first = true;

            for (int run = 0; run < 200; run++) {
                std::vector<int> tour;
                if (m == 0) tour = random_solution(inst, rng);
                if (m == 1) tour = nearest_neighbor_end(inst, run % inst.n);
                if (m == 2) tour = nearest_neighbor_anywhere(inst, run % inst.n);
                if (m == 3) tour = greedy_cycle(inst, run % inst.n);

                long long val = tour_objective(inst, tour);
                sum += val;
                if (first || val < best) {
                    best = val;
                    best_tour = tour;
                }
                if (first || val > worst) worst = val;
                first = false;
            }

            double avg = (double)sum / 200.0;
            bool ok = check_solution(inst, best_tour, best);

            std::cout << methods[m] << "  min=" << best << "  max=" << worst
                      << "  avg=" << std::fixed << std::setprecision(2) << avg
                      << "  checker=" << (ok ? "OK" : "FAIL") << "\n";

            out << inst.name << " " << methods[m] << " " << best << " " << worst << " "
                << std::fixed << std::setprecision(2) << avg << " "
                << (ok ? "OK" : "FAIL") << "\n";
            out << "tour:";
            for (int i = 0; i < (int)best_tour.size(); i++) {
                out << " " << best_tour[i];
            }
            out << "\n";
        }
        std::cout << "\n";
    }

    std::cout << "Wrote results.txt\n";
    return 0;
}
