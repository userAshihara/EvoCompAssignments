"""Draw 2D plots of the best tours from results.txt using pandas."""
import os
import pandas as pd


def load_csv(path):
    return pd.read_csv(path, sep=";", names=["x", "y", "cost"])


def load_results(path):
    lines = open(path).read().splitlines()
    solutions = []
    i = 1
    while i < len(lines):
        parts = lines[i].split()
        inst = parts[0]
        method = parts[1]
        best = int(parts[2])
        tour = [int(x) for x in lines[i + 1].replace("tour:", "").split()]
        solutions.append((inst, method, best, tour))
        i += 2
    return solutions


def draw(ax, nodes, tour, title):
    selected = nodes.iloc[tour]
    unused = nodes.drop(index=tour)

    unused.plot.scatter(x="x", y="y", color="lightgray", s=12, ax=ax, legend=False)

    cycle = selected[["x", "y"]]
    cycle = pd.concat([cycle, cycle.iloc[[0]]], ignore_index=True)
    cycle.plot(x="x", y="y", color="steelblue", linewidth=1, ax=ax, legend=False)

    selected = selected.copy()
    selected["size"] = 12 + selected["cost"] / 8
    selected.plot.scatter(
        x="x", y="y", c="cost", s="size", colormap="viridis",
        colorbar=False, ax=ax, legend=False,
    )

    ax.set_title(title)
    ax.set_aspect("equal")
    ax.set_xlabel("")
    ax.set_ylabel("")
    ax.set_xticks([])
    ax.set_yticks([])


def main():
    data = {
        "TSPA": load_csv("../TSPA.csv"),
        "TSPB": load_csv("../TSPB.csv"),
    }
    solutions = load_results("results.txt")
    os.makedirs("graphs", exist_ok=True)

    pretty = {
        "random": "Random",
        "nearest_neighbor_end": "NN (end)",
        "nearest_neighbor_anywhere": "NN (anywhere)",
        "greedy_cycle": "Greedy cycle",
    }

    for inst in ["TSPA", "TSPB"]:
        nodes = data[inst]
        ax0 = nodes.head(0).plot.scatter(x="x", y="y", figsize=(12, 8), subplots=False)
        fig = ax0.figure
        fig.clear()
        axes = fig.subplots(2, 2)
        k = 0
        for name, method, best, tour in solutions:
            if name != inst:
                continue
            draw(axes[k // 2][k % 2], nodes, tour, pretty[method] + ": " + str(best))
            k += 1
        fig.suptitle(inst + " best solutions")
        fig.savefig("graphs/" + inst + "_best.png", dpi=130, bbox_inches="tight")

    print("okay big bro heres your graphs")


if __name__ == "__main__":
    main()
