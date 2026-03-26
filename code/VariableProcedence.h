#ifndef VARIABLE_PROCEDENCE_H
#define VARIABLE_PROCEDENCE_H

#include <vector>

namespace openwbo {

class VariableProcedence {
public:
    static const int RED    = 0x01;
    static const int BLUE   = 0x02;
    static const int PURPLE = 0x03;
    static const int COLOR_BIT_LENGTH = 2;

    explicit VariableProcedence(int n_vars) : color(n_vars, 0) {}

    void markAsBlue(int variable)   { color[variable] = BLUE; }
    void markAsPurple(int variable) { color[variable] = PURPLE; }
    void markAsRed(int variable)    { color[variable] = RED; }

    int getColor(int variable) const { return color[variable]; }

private:
    std::vector<int> color;
};

} // namespace openwbo

#endif // VARIABLE_PROCEDENCE_H
