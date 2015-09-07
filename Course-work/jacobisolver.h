#ifndef JACOBISOLVER
#define JACOBISOLVER

#include "SystemSolver.h"


class JacobiSolver:public SystemSolver {
public:
    int solve() override;
    int solve(double eps) override;
    bool isConvergent() override;
    double nextIteration() override;

    JacobiSolver(LinearSystem& sys);
};



#endif // JACOBISOLVER

