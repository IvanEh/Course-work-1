#include "JacobiSolver.h"
#include <string>
#include <math.h>

int JacobiSolver::solve()
{
    return solve(0.001);
}

int JacobiSolver::solve(double eps)
{
    if (system.getVariablesCount() != system.getEquationCount())
        return -1;

    int n = system.getVariablesCount();
    X.resize(n);
    for (int i = 0; i < X.size(); i++)
    {
        X[i] = 0;
    }

    double maxLocalEps = 0;
    int iter = 0;

    do
    {
        iter++;

        maxLocalEps = 0;
        DVector newSolution = DVector(n, 0);
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
            newX = (newX - other)/ system.getCoefficient(i, i);
            newSolution[i] = newX;

            if(newX != newX){
                fail = true;
                return -1;
            }
        }

        for (int i = 0; i < n; i++)
        {
            double t = fabs(X[i] - newSolution[i]);
            if (t > maxLocalEps)
                maxLocalEps = t;
        }
        X = newSolution;
    }while(maxLocalEps >= eps);

    solved = true;
    return iter;
}



bool JacobiSolver::isConvergent()
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

double JacobiSolver::nextIteration()
{
    if (system.getVariablesCount() != system.getEquationCount())
        return -1;

    int n = system.getVariablesCount();
    X.resize(n);

    double maxLocalEps = 0;
    int iter = 0;

    iter++;

    maxLocalEps = 0;
    DVector newSolution = DVector(n, 0);
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

        if(newX != newX){
            fail = true;
            return -1;
        }

        newSolution[i] = newX;
    }

    for (int i = 0; i < n; i++)
    {
        double t = fabs(X[i] - newSolution[i]);
        if (t > maxLocalEps)
            maxLocalEps = t;
    }
    X = newSolution;

    return maxLocalEps;
}

JacobiSolver::JacobiSolver(LinearSystem& sys) :SystemSolver(sys)
{

}

