#include "SystemSolver.h"
#include <string> //

bool SystemSolver::isSolved() {
    return solved;
}

Solution &SystemSolver::getSolution() {
    return X;
}

double SystemSolver::execIter(int iter)
{
    for(int i = 0; i < iter-1; i++){
        nextIteration();
    }
    return nextIteration();
}

bool SystemSolver::failed()
{
    return fail;
}

SystemSolver::SystemSolver(LinearSystem &sys)
    :system(sys){
    X = Solution();
    system = sys;
    solved = false;
    fail = false;
}


string SystemSolver::toString()
{
    string mess = "( ";
    for (auto x : X)
    {
        mess.append(to_string(x));
        mess.append(" ");
    }
    mess.append(")");

    return mess;
}
