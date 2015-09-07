#include "GaussSeidelSolver.h"
#include <cmath>

int GaussSeidelSolver::solve()
{
    return solve(0.001);
}

int GaussSeidelSolver::solve(double eps)
{
    if (system.getVariablesCount() != system.getEquationCount())
        return -1;

    int n = system.getVariablesCount();
    X.resize(n);
    for (int i = 0; i < X.size(); i++)
    {
        X[i] = 0;
    }

    double maxLocalEps;
    int iter = 0;
    do
    {
        iter++;
        maxLocalEps = 0;
        for (int i = 0; i < n; i++)
        {
            double b_i = system.getFreeTerm(i);
            double newX = b_i;
            double other = 0;
            for (int j = 0; j < n; j++)
            {
                if (i != j)
                {
                    other += system.getCoefficient(i, j)*X[j];
                }
            }
            newX = (newX - other) / system.getCoefficient(i, i);
            double delta = fabs(newX - X[i]);
            if (delta > maxLocalEps)
                maxLocalEps = delta;

            if(newX != newX){
                fail = true;
                return -1;
            }

            X[i] = newX;
        }

    } while (maxLocalEps >= eps);

    return iter;
}

bool GaussSeidelSolver::isConvergent()
{
    int  n = system.getVariablesCount();
    for (int i = 0; i < n; i++)
    {
        double other = 0;
        for (int j = 0; j < n; j++)
        {
            if (i != j)
                other += system.getCoefficient(i,j);
        }
        if (fabs(system.getCoefficient(i, i)) < fabs(other))
            return false;
    }

    return true;
}

double GaussSeidelSolver::nextIteration()
{
    if (system.getVariablesCount() != system.getEquationCount())
        return -1;
    int n = system.getVariablesCount();
    X.resize(n);

    double maxLocalEps;
    int iter = 0;

    iter++;
    maxLocalEps = 0;
    for (int i = 0; i < n; i++)
    {
        double b_i = system.getFreeTerm(i);
        double newX = b_i;
        double other = 0;
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                other += system.getCoefficient(i, j)*X[j];
            }
        }
        newX = (newX - other) / system.getCoefficient(i, i);
        double delta = fabs(newX - X[i]);
        if (delta > maxLocalEps)
            maxLocalEps = delta;

        if(newX != newX){
            fail = true;
            return -1;
        }

        X[i] = newX;
    }

    return maxLocalEps;
}

GaussSeidelSolver::GaussSeidelSolver(LinearSystem& sys) :SystemSolver(sys)
{
}

GaussSeidelSolver::~GaussSeidelSolver()
{
}
