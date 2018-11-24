#include "libSimpleMath.hpp"

extern "C" {
    smath::SimpleMath* smath_new() {
        printf("C interfcae says hi!\n");
        return new smath::SimpleMath();
    }
    int smath_add_eq(smath::SimpleMath* smath, char* inEq, char* inVars){
        string_t  theEQ;
        vstring_t theVars;
        return smath->addEquation(theEQ, theVars);
    }
}
