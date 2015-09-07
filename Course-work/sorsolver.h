#ifndef SORSOLVER
#define SORSOLVER

#include "SystemSolver.h"

class SORSolver: public SystemSolver
{
protected:
    int convergenceFailThreshold = 25;
    bool inited = false;
    double w = 1.14;
    bool convergenceFail = false;
public:
    int solve() override;
    int solve(double eps) override;
    bool isConvergent() override;
    double nextIteration() override;

    /**
     * Встановлює коефіціент релаксації
     * @param factor - коефіціент релаксації
     */
    void setRelaxationFactor(double factor);

    /**
     * Встановлює максимальну кількість операцій,<br>
     * які можуть бути виконані без покращення точності
     * @param c - поріг максимальної кількості ітерацій
     */
    void setConvergenceFailThreshold(int c);

    /**
     * Визначає чи метод релаксації зійшовся.<br>
     * Викликається тільки після операції solve
     * @return
     */
    bool converged();
public:
    SORSolver(LinearSystem& sys);
    ~SORSolver();
};



#endif // SORSOLVER

