/**
 *  Equation Nibbler Library
 * ==========================
 *  A math function parser using a reverse polish notation algorithm.
 *  Loosely based on: http://www.codeproject.com/Articles/345888/How-to-write-a-simple-interpreter-in-JavaScript
 */

#include "libEqNibbler.hpp"

using namespace std;
using namespace eqnibbler;

EquationNibbler::EquationNibbler() {

}

EquationNibbler::~EquationNibbler() {

}

size_t EquationNibbler::addEquation(string_t sEquation, vstring_t vsVariable) {

    m_Eqs.push_back(new Math());
    size_t newEq = m_Eqs.size() - 1;

    m_Eqs[newEq]->setEquation(sEquation);
    m_Eqs[newEq]->setVariables(vsVariable);

    return newEq;

}
