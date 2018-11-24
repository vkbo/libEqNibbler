/**
 *  Equation Nibbler Library
 * ==========================
 *  A math function parser using a reverse polish notation algorithm.
 *  Loosely based on: http://www.codeproject.com/Articles/345888/How-to-write-a-simple-interpreter-in-JavaScript
 */

#include "clsMath.hpp"

namespace smath {

class SimpleMath {

    public:

    SimpleMath();
    ~SimpleMath();

    size_t   addEquation(string_t, vstring_t);
    double_t evalEquation(size_t, vdouble_t);

    private:

    std::vector<Math*> m_Eqs;

};

} // End NameSpace
