#ifndef PB_SOLUTION_H
#define PB_SOLUTION_H

#include <vector>

namespace openwbo {

class PBSolution {
private:
    std::vector<bool> bits;

public:
    explicit PBSolution(int n_vars) : bits(n_vars, false) {}
    PBSolution(const std::vector<bool>& b) : bits(b) {}

    int size() const { return (int)bits.size(); }

    bool getBit(int index) const { return bits[index]; }
    void setBit(int index, bool value) { bits[index] = value; }

    const std::vector<bool>& getBits() const { return bits; }
    void setBits(const std::vector<bool>& b) { bits = b; }
};
}

#endif // PB_SOLUTION_H

