#include "PartitionCrossover.h"
#include <unordered_set>
#include <queue>
#include <random>

namespace openwbo {

PartitionCrossover::PartitionCrossover(EmbeddedLandscape_maxsat &landscape)
    : el(landscape), rng(std::random_device{}()), lastRuntime(0),
      numberOfComponents(0), vp(landscape.getN()) {}

bool PartitionCrossover::isNodeInReducedGraph(int v,
                                              const PBSolution &blue,
                                              const PBSolution &red) const {
    return blue.getBit(v) != red.getBit(v);
}

std::vector<int> PartitionCrossover::bfsComponent(int start,
                                                  const PBSolution &blue,
                                                  const PBSolution &red,
                                                  std::vector<char> &seen) {
    std::queue<int> q;
    std::vector<int> component;
    q.push(start);
    seen[start] = 1;

    while (!q.empty()) {
        int v = q.front(); q.pop();
        component.push_back(v);

        el.forEachNeighbor(v, [&](int u) {
            if (!seen[u] && isNodeInReducedGraph(u, blue, red)) {
                seen[u] = 1;
                q.push(u);
            }
        });
    }

    return component;
}

std::pair<double, double> PartitionCrossover::evaluateComponent(
    const std::vector<int> &component,
    const PBSolution &blue,
    const PBSolution &red) 
{
    std::unordered_set<int> subfns;
    for (int var : component) {
        const auto &clauses = el.getClausesOfVar(var);
        subfns.insert(clauses.begin(), clauses.end());
    }

    double valBlue = 0.0, valRed = 0.0;
    for (int c : subfns) {
        valBlue += el.evaluateSubFunction(c, blue);
        valRed  += el.evaluateSubFunction(c, red);
    }

    return {valBlue, valRed};
}

PBSolution PartitionCrossover::recombine(const PBSolution &blue,
                                         const PBSolution &red) 
{
    auto startTime = std::chrono::high_resolution_clock::now();

    const int n = el.getN();
    PBSolution child = red; // Copia inicial de red

    VariableProcedence vp(n);
    for (int i = 0; i < n; ++i) {
        if (blue.getBit(i) == red.getBit(i)) vp.markAsPurple(i);
        else if (blue.getBit(i)) vp.markAsBlue(i);
        else vp.markAsRed(i);
    }

    std::vector<char> seen(n, 0);
    numberOfComponents = 0;

    for (int v = 0; v < n; ++v) {
        if (seen[v] || !isNodeInReducedGraph(v, blue, red)) continue;

        auto component = bfsComponent(v, blue, red, seen);
        auto [valBlue, valRed] = evaluateComponent(component, blue, red);

        bool chooseBlue = (valBlue > valRed) || 
                          (valBlue == valRed && std::uniform_real_distribution<>(0.0,1.0)(rng) < 0.5);

        if (chooseBlue) {
            for (int var : component) {
                child.setBit(var, blue.getBit(var));
                vp.markAsBlue(var);
            }
        } else {
            for (int var : component) vp.markAsRed(var);
        }

        numberOfComponents++;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    lastRuntime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

    return child;
}

int PartitionCrossover::getNumberOfComponents() const {
    return numberOfComponents;
}

long long PartitionCrossover::getLastRuntime() const {
    return lastRuntime;
}

VariableProcedence PartitionCrossover::getVarProcedence() const {
    return vp; // requiere que vp sea miembro si quieres acceder fuera
}

} // namespace openwbo
