#include "EmbeddedLandscape_maxsat.h"
#include <algorithm>
#include <cstdio>
#include <cmath>

namespace openwbo {

EmbeddedLandscape_maxsat::EmbeddedLandscape_maxsat(int numVars)
    : nVars(numVars), appearsIn_(numVars), interactions_(numVars),
      interactionsSet_(numVars), subSolution_(numVars) {}

int EmbeddedLandscape_maxsat::getN() const { return nVars; }
int EmbeddedLandscape_maxsat::getNumClauses() const { return nClauses; }

int EmbeddedLandscape_maxsat::getMaximumDegreeOfVIG() const {
    if (maximumDegreeOfVIG < 0) {
        maximumDegreeOfVIG = 0;
        for (int i = 0; i < nVars; i++) {
            if (interactions_[i].size() > (size_t)maximumDegreeOfVIG)
                maximumDegreeOfVIG = interactions_[i].size();
        }
    }
    return maximumDegreeOfVIG;
}

void EmbeddedLandscape_maxsat::addClause(const std::vector<int>& vars) {
    clauses_.push_back(vars);
    int idx = (int)clauses_.size() - 1;

    for (int v : vars) {
        if (v < 0 || v >= nVars) continue;
        appearsIn_[v].push_back(idx);
    }

    for (size_t i = 0; i < vars.size(); i++) {
        int vi = vars[i];
        for (size_t j = 0; j < vars.size(); j++) {
            int vj = vars[j];
            if (vi != vj) interactionsSet_[vi].insert(vj);
        }
    }

    nClauses++;
}

void EmbeddedLandscape_maxsat::addClauseweight(const std::vector<int>& vars, uint64_t weight) {
    clauses_.push_back(vars);
    clauseWeights_.push_back(weight);
    int idx = (int)clauses_.size() - 1;

    for (int v : vars) {
        if (v < 0 || v >= nVars) continue;
        appearsIn_[v].push_back(idx);
    }

    for (size_t i = 0; i < vars.size(); i++) {
        int vi = vars[i];
        for (size_t j = 0; j < vars.size(); j++) {
            int vj = vars[j];
            if (vi != vj) interactionsSet_[vi].insert(vj);
        }
    }

    nClauses++;
}

void EmbeddedLandscape_maxsat::buildInteractionGraph() {
    for (int v = 0; v < nVars; v++) {
        interactions_[v] = std::vector<int>(interactionsSet_[v].begin(), interactionsSet_[v].end());
        std::sort(interactions_[v].begin(), interactions_[v].end());
    }
}

const std::vector<int>& EmbeddedLandscape_maxsat::getClausesOfVar(int var) const {
    return appearsIn_[var];
}

const std::vector<int>& EmbeddedLandscape_maxsat::getInteractions(int var) const {
    return interactions_[var];
}

const std::vector<int>& EmbeddedLandscape_maxsat::getMask(int clauseIdx) const {
    return clauses_[clauseIdx];
}

void EmbeddedLandscape_maxsat::forEachNeighbor(int var, std::function<void(int)> f) const {
    for (int u : interactions_[var]) f(u);
}

double EmbeddedLandscape_maxsat::evaluateCompleteSolution(const PBSolution& sol) const {
    double score = 0.0;
    for (const auto& clause : clauses_) {
        bool satisfied = false;
        for (int lit : clause) {
            int var = std::abs(lit) - 1;
            bool value = sol.getBit(var);
            if ((lit > 0 && value) || (lit < 0 && !value)) {
                satisfied = true;
                break;
            }
        }
        if (satisfied) score += 1.0;
    }
    return score;
}

double EmbeddedLandscape_maxsat::evaluateCompleteSolutionweight(const PBSolution& sol) const {
    double score = 0.0;
    for (size_t idx = 0; idx < clauses_.size(); idx++) {
        const auto& clause = clauses_[idx];
        bool satisfied = false;
        for (int lit : clause) {
            int var = std::abs(lit) - 1;
            bool value = sol.getBit(var);
            if ((lit > 0 && value) || (lit < 0 && !value)) {
                satisfied = true;
                break;
            }
        }
        if (satisfied) score += (double)clauseWeights_[idx];
    }
    return score;
}

double EmbeddedLandscape_maxsat::evaluateSubFunction(int clauseIdx, const PBSolution& sol) const {
    const auto& clause = clauses_[clauseIdx];
    bool satisfied = false;
    for (int lit : clause) {
        int var = std::abs(lit) - 1;
        if ((lit > 0 && sol.getBit(var)) || (lit < 0 && !sol.getBit(var))) {
            satisfied = true;
            break;
        }
    }
    return satisfied ? 1.0 : 0.0;
}

double EmbeddedLandscape_maxsat::evaluateSubFunctionweight(int clauseIdx, const PBSolution& sol) const {
    const auto& clause = clauses_[clauseIdx];
    bool satisfied = false;
    for (int lit : clause) {
        int var = std::abs(lit) - 1;
        if ((lit > 0 && sol.getBit(var)) || (lit < 0 && !sol.getBit(var))) {
            satisfied = true;
            break;
        }
    }
    return satisfied ? (double)clauseWeights_[clauseIdx] : 0.0;
}


double EmbeddedLandscape_maxsat::evaluateSubFunctionFlippingVariable(int clauseIdx, int var, const PBSolution& sol) const {
    const auto& clause = clauses_[clauseIdx];
    bool satisfied = false;
    for (int lit : clause) {
        int v = std::abs(lit) - 1;
        bool value = (v == var) ? !sol.getBit(v) : sol.getBit(v);
        if ((lit > 0 && value) || (lit < 0 && !value)) {
            satisfied = true;
            break;
        }
    }
    return satisfied ? 1.0 : 0.0;
}


double EmbeddedLandscape_maxsat::evaluateSubFunctionFlippingVariableWeight(int clauseIdx, int var, const PBSolution& sol) const {
    
    const auto& clause = clauses_[clauseIdx];
    bool satisfied = false;
    for (int lit : clause) {
        int v = std::abs(lit) - 1;
        bool value = (v == var) ? !sol.getBit(v) : sol.getBit(v);
        if ((lit > 0 && value) || (lit < 0 && !value)) {
            satisfied = true;
            break;
        }
    }
    return satisfied ? (double)clauseWeights_[clauseIdx] : 0.0;
}


double EmbeddedLandscape_maxsat::evaluateSubFunctionFlippingAllVariables(int clauseIdx, const PBSolution& sol) const {
    const auto& clause = clauses_[clauseIdx];
    bool satisfied = false;
    for (int lit : clause) {
        int var = std::abs(lit) - 1;
        bool value = !sol.getBit(var);
        if ((lit > 0 && value) || (lit < 0 && !value)) {
            satisfied = true;
            break;
        }
    }
    return satisfied ? 1.0 : 0.0;
}

double EmbeddedLandscape_maxsat::evaluateSubFunctionFlippingAllVariablesWeight(
    int clauseIdx, const PBSolution& sol) const {
    
    const auto& clause = clauses_[clauseIdx];
    bool satisfied = false;
    for (int lit : clause) {
        int var = std::abs(lit) - 1;
        bool value = !sol.getBit(var);
        if ((lit > 0 && value) || (lit < 0 && !value)) {
            satisfied = true;
            break;
        }
    }
    return satisfied ? (double)clauseWeights_[clauseIdx] : 0.0;
}


void EmbeddedLandscape_maxsat::printSummary(int maxClauses, int maxVars) const {
    printf("Número de cláusulas: %zu\n", clauses_.size());
    for (size_t i = 0; i < clauses_.size() && i < (size_t)maxClauses; i++) {
        printf("Cláusula %zu: ", i);
        for (int v : clauses_[i]) printf("%d ", v);
        printf("\n");
    }

    printf("\nInteracciones (hasta 10 variables con interacciones):\n");
    int count = 0;
    for (int v = 0; v < nVars && count < 10; v++) {
        if (!interactions_[v].empty()) {
            printf("Variable %d interactúa con: ", v);
            for (int u : interactions_[v]) printf("%d ", u);
            printf("\n");
            count++;
        }
    }
}

} // namespace openwbo


