#ifndef GAUSSIANELIMINATION
#define GAUSSIANELIMINATION
#include "SystemSolver.h"

class GaussianElimination: public SystemSolver
{
public:
    int solve() override;
    int solve(double eps) override;
    bool isConvergent() override;
    double nextIteration() override;
public:
//    GaussianElimination();
    GaussianElimination(LinearSystem& solver);
    ~GaussianElimination();
protected:
    static void swap(double ** A, int cols, int row1, int row2 );
};

#endif // GAUSSIANELIMINATION

