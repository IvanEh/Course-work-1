
//
// Created by Ivan on 11.05.2015.
//

#include <iostream> // Виведення в консоль
#include "linear-system.h"
#include <string> // Обробка послідовності символів

LinearSystem::LinearSystem(int m, DMatrix &coeff, DVector &res)
{
    this->m = m;
    this->coeffs = DMatrix(coeff);
    this->freeTerms = DVector(res);
}

LinearSystem::LinearSystem(int m, initializer_list<initializer_list<double>> coeff, initializer_list<double> res)
{
    for (auto lst : coeff)
    {
        if (lst.size() != m)
        {
            this->m = 0;
            this->coeffs.resize(0);
            this->freeTerms.resize(0);
            return;
        }
    }

    coeffs.resize(m);
    int i = 0;
    for (auto lst : coeff)
    {
        coeffs[i].resize(m);
        int j = 0;
        for (auto x : lst)
        {
            coeffs[i][j] = x;
            j++;
        }
        i++;
    }

    freeTerms.resize(m);
    i = 0;
    for (auto el : res)
    {
        freeTerms[i] = el;
        i++;
    }

    this->m = m;
}

LinearSystem::LinearSystem()
: freeTerms(*(new vector<double>)),
    coeffs(*(new vector<vector<double> >()))
{
}

Equation & LinearSystem::getEquation(int i) {
    return coeffs[i];
}

double LinearSystem::setCoefficient(int i, int j, double val) {
    double prev = getEquation(i)[j];
    getEquation(i)[j] = val;
    return prev;
}

int LinearSystem::getEquationCount() {
    return coeffs.size();
}

int LinearSystem::getVariablesCount()
{
    return m;
}

string LinearSystem::toString()
{
    string str = "";
    int j = 0;
    for (auto lst : coeffs)
    {
        int i = 1;
        for (auto x : lst)
        {
            if (x != 0)
            {
                str.append(to_string(x));
                str.append("x");
                str.append(to_string(i));
                str.append(" ");
            }
            i++;
        }

        str.append(" = ");
        str.append(to_string(freeTerms[j]));
        str.append("\n");
        j++;
    }

    return str;
}

double LinearSystem::getFreeTerm(int i)
{
    return freeTerms[i];
}

double LinearSystem::getCoefficient(int i, int j)
{
    return getEquation(i)[j];
}
