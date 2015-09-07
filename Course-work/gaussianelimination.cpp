#include "GaussianElimination.h"
#include <QtGlobal>

int GaussianElimination::solve()
{
    nextIteration();
    return 1;
}

int GaussianElimination::solve(double eps)
{
    return solve();
}

bool GaussianElimination::isConvergent()
{
    if(X.size() == 0){
        fail = true;
        return false;
    }
     fail = X[0] != X[0];
     for(int i = 0; i < X.size(); i++){
         if(qInf() == qAbs(X[i]))
         {
             fail = true;
             break;
         }
     }
    return !fail;
}

double GaussianElimination::nextIteration()
{
        int i, j, k, n;
        n = system.getVariablesCount();


        X.resize(n);
        for (int i = 0; i < X.size(); i++)
        {
            X[i] = 0;
        }

        if(n == 0)
            return 0;

        double** A = new double*[n+1];
        for (int i = 0; i <= n; i++)
        {
            A[i] = new double[n + 2];
        }
        float c, sum = 0.0;

        for (i = 1; i <= n; i++)
        {
            for (j = 1; j <= n; j++)
            {
                A[i][j] = system.getCoefficient(i-1, j-1);
            }
        }

        for (int i = 1; i <= n; i++)
        {
            int j = n + 1;
            A[i][j] = system.getFreeTerm(i-1);
        }

        for (j = 1; j <= n; j++)
        {
            for(int l = j; l <= n; l++){
                if(A[l][j] != 0){
                    swap(A, n+2, j, l);
                    break;
                }
            }
            for (i = 1; i <= n; i++)
            {
                if (i>j)
                {
                    c = A[i][j] / A[j][j];
                    for (k = 1; k <= n + 1; k++)
                    {
                        A[i][k] = A[i][k] - c*A[j][k];
                    }
                }
            }
        }
        X[n - 1] = A[n][n + 1] / A[n][n];

        for (i = n - 1; i >= 1; i--)
        {
            sum = 0;
            for (j = i + 1; j <= n; j++)
            {
                sum = sum + A[i][j] * X[j-1];
            }
            X[i-1] = (A[i][n + 1] - sum) / A[i][i];
        }


        for (int i = 0; i <= n; i++)
        {
            delete[] A[i];
        }
        delete[] A;

        return 0;
}


GaussianElimination::GaussianElimination(LinearSystem& solver) :SystemSolver(solver)
{
}

GaussianElimination::~GaussianElimination()
{
}

void GaussianElimination::swap(double **A, int cols, int row1, int row2)
{
    for(int j = 0; j < cols; j++){
//        int p1 = row1*cols + j;
//        int p2 = row2*cols + j;
        double t = A[row1][j];
        A[row1][j] = A[row2][j];
        A[row2][j] = t;
//        std::swap(*A[p1],*A[p2]);
    }
}
