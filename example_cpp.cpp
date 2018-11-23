/**
 *  Equation Nibbler Library
 * ==========================
 *  C++ Example Code
 */

#include <cstdlib>

#include "source/libEqNibbler.hpp"

using namespace std;
using namespace eqnibbler;

int main(int argc, char const *argv[]) {

    EquationNibbler* theEQ = new EquationNibbler();

    vector<string> theVars{"x"};

    theEQ->addEquation("sin(x)",theVars);

    return 0;
}
