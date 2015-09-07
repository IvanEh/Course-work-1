#ifndef SYSTEMSOLVER
#define SYSTEMSOLVER

#include <vector>
#include "linear-system.h"

using namespace std;
using Solution = vector<double>;

class SystemSolver {
protected:
    LinearSystem& system;
    Solution X;
    bool solved;
    bool fail;
public:
    SystemSolver(LinearSystem &system);

    /**
     * Визначає точний або наближений розв’язок СЛАР<br>
     * Перед тим як використовувати потрібно перевірити чи метод збіжний<br>
     * методом isConvergent
     * @return кількість виконаних ітерацій для уточнення
     */
    virtual int solve() = 0;

    /**
     * Повертає відформатоване представлення розв’язку СЛАР<br>
     * у вигляді рядка
     * @return
     */
    string toString();

    /**
     * Визначає наближений розв’язок із заданою точністю
     * @param eps - задана точність
     * @return  кількість виконаних ітерацій для уточнення
     */
    virtual int solve(double eps) = 0;

    /**
     * Визначає чи метод solve був успішно виконаний
     * @return true - якщо метод успішно виконався
     */
    bool isSolved();

    /**
     * Повертає вектор розв’язку СЛАР
     * @return вектор розв’язку СЛАР
     */
    Solution& getSolution();

    /**
     * Визначає збіжність методу для заданої СЛАР
     * @return true - якщо метод збігається на заданій СЛАР
     */
    virtual bool isConvergent() = 0;

    /**
     * Обчислює наближені корені СЛАР для наступної ітерації
     * @return точнісь з якою обчислені розв'язки
     */
    virtual double nextIteration() = 0;

    /**
     * Виконує i ітерацій алгоритму.<br>
     * В загальному випадку і разів  викликає метод nextIteration()
     * @param iter - кількість ітерацій
     * @return точність
     */
    double execIter(int iter);
    bool failed();
};

#endif // SYSTEMSOLVER

