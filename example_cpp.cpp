/**
 *  Equation Nibbler Library
 * ==========================
 *  C++ Example Code
 */

#include <time.h>
#include <cstdlib>

#include "source/libEqNibbler.hpp"

using namespace std;
using namespace eqnibbler;

int main(int argc, char const *argv[]) {

    double_t theResult;
    EquationNibbler* theEQ = new EquationNibbler();
    clock_t tStart = clock();
    vector<string>   theVars{"a","x","y","z"};
    // size_t idEQ = theEQ->addEquation("-(1 + (2 + x)) + (3 + (4 + y)) + (5 + (6 + (7 + z)))",theVars);
    size_t idEQ = theEQ->addEquation("-3.2^3 + sin(pi/2) * cos(a) * exp(pi/2) - if(pi > 3, pi, 0) -(1 + (2 + x)) + (3 + (4 + y)) + (5 + (6 + (7 + z)))",theVars);
    // size_t idEQ = theEQ->addEquation("-a + x * y / z",theVars);
    vector<double_t> theVals{0.0,1.0,2.0,3.0};

    int maxItt = 20000000;
    // int maxItt = 1;

    for(int s=0; s<maxItt; s++) {
        theResult = theEQ->evalEquation(idEQ, theVals);
    }
    printf("Result: %23.16e\n", theResult);
    printf("Total time:     %.6f s\n",      (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("Per operatrion: %.6f us\n", 1e6*(double)(clock() - tStart)/CLOCKS_PER_SEC/maxItt);

    return 0;
}
