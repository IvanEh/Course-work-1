#ifndef SYSTEMLINEAREQUATION_H
#define SYSTEMLINEAREQUATION_H
#include <vector> // для роботи з вектором
#include <initializer_list> // для швидкої ініціалізція
#include <string> // для представлення послідовності символів
using namespace std;

using DMatrix = std::vector<vector<double>>;

using DVector = std::vector<double>;
using Equation = std::vector<double>;

class LinearSystem {
protected:
    DMatrix coeffs = DMatrix(); // system representation
    DVector freeTerms = DVector();
    int m; // coefficient count
public:
    LinearSystem(int m, DMatrix &coeff, DVector &res);
    LinearSystem(int m, initializer_list<initializer_list<double> > coeff , initializer_list<double> res);
    LinearSystem();

    /**
     * Отримати вектор коефіціентів і-го рівняння СЛАР
     * @param i
     * @return
     */
    Equation & getEquation(int i);

    /**
     * Встановлює j-ий коефіціент і-го рівняння. Присвоює йому значення val
     * @param i - рівняння
     * @param j - коефіціент
     * @param val - нове значення
     * @return попереднє значення
     */
    double setCoefficient(int i, int j, double val);

    /**
     * Визначає кількість рівнянь СЛАР
     * @return кількість рівнянь СЛАР
     */
    int getEquationCount() ;

    /**
     * Визначає кількість змінних СЛАР
     * @return кількість змінних СЛАР
     */
    int getVariablesCount();

    /**
     * Повертає форматоване представлення СЛАР у вигляді рядка
     * @return форматоване представлення СЛАР у вигляді рядка
     */
    string toString();

    /**
     * Повертає j-ий коефіціент i-го рівняння СЛАР
     * @param i i-го рівняння
     * @param j j-ий коефіціент
     * @return
     */
    double getCoefficient(int i, int j);

    /**
     * Повертає вільний член і-го рівняння СЛАР
     * @param i
     * @return
     */
    double getFreeTerm(int i);
    
};


#endif // SYSTEMLINEAREQUATION_H

