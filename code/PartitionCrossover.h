#ifndef PARTITIONCROSSOVER_H
#define PARTITIONCROSSOVER_H

#include "EmbeddedLandscape_maxsat.h"
#include "PBSolution.h"
#include "VariableProcedence.h"
#include <vector>
#include <random>
#include <chrono>

namespace openwbo {

class PartitionCrossover {
public:
    PartitionCrossover(EmbeddedLandscape_maxsat &landscape);

    // Recombina dos soluciones padre
    PBSolution recombine(const PBSolution &blue, const PBSolution &red);

    int getNumberOfComponents() const;
    long long getLastRuntime() const; // nanosegundos
    VariableProcedence getVarProcedence() const;

private:
    EmbeddedLandscape_maxsat &el;
    std::mt19937 rng;

    long long lastRuntime;
    int numberOfComponents;

    VariableProcedence vp; // Procedencia de variables

    bool isNodeInReducedGraph(int v, const PBSolution &blue, const PBSolution &red) const;

    // BFS para encontrar una componente conectada en el grafo reducido
    std::vector<int> bfsComponent(int start,
                                  const PBSolution &blue,
                                  const PBSolution &red,
                                  std::vector<char> &seen);

    // Evalúa una componente de variables en ambas soluciones
    std::pair<double, double> evaluateComponent(const std::vector<int> &component,
                                                const PBSolution &blue,
                                                const PBSolution &red);
};

} // namespace openwbo

#endif // PARTITIONCROSSOVER_H

