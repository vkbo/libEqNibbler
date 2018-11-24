#include "libSimpleMath.hpp"

extern "C" {
    smath::SimpleMath* py_smath_init() {
        printf("C interface says hi!\n");
        return new smath::SimpleMath();
    }
    int py_smath_add_eq(smath::SimpleMath* smath, char* inEq, char* inVars){
        string_t  theEQ;
        vstring_t theVars;
        return smath->addEquation(theEQ, theVars);
    }
    double py_smath_eval_eq(smath::SimpleMath* smath, int inID, double inVals) {
        vdouble_t theVals;
        return smath->evalEquation((size_t)inID, theVals);
    }
}
