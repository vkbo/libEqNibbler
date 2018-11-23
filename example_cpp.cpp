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

    vector<string>   theVars{"x"};
    vector<double_t> theVals{1.0};

    size_t idEQ = theEQ->addEquation("-sin(x)*1e2 + (+5e+1)",theVars);
    // size_t idEQ = theEQ->addEquation("-if(x >= 1, 1, 0)",theVars);

    double_t theResult = theEQ->evalEquation(idEQ, theVals);

    printf("Result: %23.16e\n", theResult);

    return 0;
}
