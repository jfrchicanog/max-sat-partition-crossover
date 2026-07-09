#include "PBSolution.h"

namespace openwbo {
std::ostream& operator<<(std::ostream& os, const PBSolution& solution) {
    for (int i = 0; i < solution.size(); ++i) {
        os << (solution.getBit(i) ? '1' : '0');
    }
    return os;
}
}
