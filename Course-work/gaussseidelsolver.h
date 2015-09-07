#ifndef GAUSSSEIDELSOLVER
#define GAUSSSEIDELSOLVER

#include "SystemSolver.h"

class GaussSeidelSolver:public SystemSolver
{
public:
    int solve() override;
    int solve(double eps) override;
    bool isConvergent() override;
    double nextIteration() override;

public:
    GaussSeidelSolver(LinearSystem& sys);
    ~GaussSeidelSolver();
};



#endif // GAUSSSEIDELSOLVER

