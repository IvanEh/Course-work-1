#include "SORSolver.h"
#include <QtGlobal>
#include <cmath>

int SORSolver::solve()
{
    return solve(0.001);
}

int SORSolver::solve(double eps)
{
    double currentEps;
    int iter = 0;
    int increasing = 0;
    double lastEps = -1;

    if(system.getVariablesCount() == 0)
        return 0;

    do
    {
        if(increasing > convergenceFailThreshold){
            convergenceFail = true;
            return iter;
        }
        lastEps = currentEps;

        currentEps = nextIteration();
        iter++;

        if(X[0] != X[0]){
            convergenceFail = true;
            return iter;
        }


        if(currentEps >= lastEps){
            increasing++;
        }else{
            increasing = 0;
        }
    } while (currentEps > eps);

    return iter;
}

bool SORSolver::isConvergent()
{
    return true;
}

double SORSolver::nextIteration()
{
    if (system.getVariablesCount() != system.getEquationCount())
        return -1;



    int n = system.getVariablesCount();
    X.resize(n);
    if (!inited)
    {
        for (int i = 0; i < X.size(); i++)
        {
            X[i] = 0;
        }
    }
    this->inited = true;

    double maxLocaleEps = 0;
    for (int i = 0; i < n; i++)
    {

        double newX;
        double x = X[i];
        double U = 0;

        newX = (1 - w)*x;
        for (int j = 0; j < n; j++)
        {
            if (j == i)
                continue;
            double a = system.getCoefficient(i, j);
            U += a*X[j];
        }
        U = -U + system.getFreeTerm(i);
        newX = newX + w / system.getCoefficient(i, i)*U;

        if(newX != newX || qInf() == qAbs(newX)){
            fail = true;
            return -1;
        }

        double delta = fabs(x - newX);

        if (delta > maxLocaleEps)
            maxLocaleEps = delta;

        X[i] = newX;
    }

    return maxLocaleEps;
}

void SORSolver::setRelaxationFactor(double factor)
{
    this->w = factor;
}

void SORSolver::setConvergenceFailThreshold(int c)
{
    convergenceFailThreshold = c;
}

bool SORSolver::converged()
{
    return !convergenceFail;
}

SORSolver::SORSolver(LinearSystem& sys) :SystemSolver(sys)
{
}

SORSolver::~SORSolver()
{
}
