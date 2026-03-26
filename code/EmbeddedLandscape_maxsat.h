#ifndef EMBEDDED_LANDSCAPE_MAXSAT_H
#define EMBEDDED_LANDSCAPE_MAXSAT_H

#include <vector>
#include <functional>
#include <unordered_set>
#include "PBSolution.h"
#include <cstdint>

namespace openwbo {

class EmbeddedLandscape_maxsat {
public:
    explicit EmbeddedLandscape_maxsat(int numVars);

    // Getters
    int getN() const;
    int getNumClauses() const;
    int getMaximumDegreeOfVIG() const;

    // Estructuras de interacción
    const std::vector<int>& getClausesOfVar(int var) const;
    const std::vector<int>& getInteractions(int var) const;
    const std::vector<int>& getMask(int clauseIdx) const;

    // Vecinos de una variable (grafo de interacción)
    void forEachNeighbor(int var, std::function<void(int)> f) const;

    // Construcción
    void addClause(const std::vector<int>& lits);
    void addClauseweight(const std::vector<int>& vars, uint64_t weight);
    uint64_t getClauseWeight(int clauseIdx) const { return clauseWeights_[clauseIdx]; }
    void buildInteractionGraph();

    // Evaluación sin peso 
    double evaluateCompleteSolution(const PBSolution& sol) const;
    double evaluateSubFunction(int clauseIdx, const PBSolution& sol) const;
    double evaluateSubFunctionFlippingVariable(int clauseIdx, int var, const PBSolution& sol) const;
    double evaluateSubFunctionFlippingAllVariables(int clauseIdx, const PBSolution& sol) const;

    // ===== Evaluación CON peso =====
    double evaluateCompleteSolutionweight(const PBSolution& sol) const;
    double evaluateSubFunctionweight(int clauseIdx, const PBSolution& sol) const;
    double evaluateSubFunctionFlippingVariableWeight(int clauseIdx, int var, const PBSolution& sol) const;
    double evaluateSubFunctionFlippingAllVariablesWeight(int clauseIdx, const PBSolution& sol) const;

    void printSummary(int maxClauses, int maxVars) const;

private:
    int nVars;
    int nClauses = 0;
    mutable int maximumDegreeOfVIG = -1;

    // Cada variable → lista de cláusulas en que aparece
    std::vector<std::vector<int>> appearsIn_;

    // Grafo de interacción
    std::vector<std::vector<int>> interactions_;
    std::vector<std::unordered_set<int>> interactionsSet_;

    // Almacena las cláusulas
    std::vector<std::vector<int>> clauses_;
    std::vector<uint64_t> clauseWeights_;

    // Subsolución temporal
    mutable PBSolution subSolution_;
};

} // namespace openwbo

#endif // EMBEDDED_LANDSCAPE_MAXSAT_H

