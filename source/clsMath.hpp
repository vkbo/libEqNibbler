/**
 *  ReyPIC – Math Class Header
 * ============================
 *  A maths parser for input functions.
 *  Base on a reverse polish notation algorithm.
 *  Loosely based on: http://www.codeproject.com/Articles/345888/How-to-write-a-simple-interpreter-in-JavaScript
 */

#ifndef CLASS_MATH
#define CLASS_MATH

// Class-specific macros
#define MT_NONE      0
#define MT_OPERATOR  1
#define MT_UNARYOP   2
#define MT_NUMBER    3
#define MT_WORD      4
#define MT_SEPARATOR 5

#define MP_INVALID  -1
#define MP_NONE      0
#define MP_NUMBER    1
#define MP_VARIABLE  2
#define MP_CONST     3
#define MP_FUNC      4
#define MP_LOGICAL   5
#define MP_MATH      6
#define MP_UNARY     7
#define MP_LBRACK    8
#define MP_RBRACK    9
#define MP_COMMA     10
#define MP_END       11

#define ASSOC_L      1
#define ASSOC_R      2

#define EVAL_TRUE    1.0
#define EVAL_FALSE   0.0

// Includes
// #include <cstdlib>
// #include <cstdint>
#include <iostream>
#include <cmath>
// #include <cstring>
// #include <array>
#include <vector>
// #include <cctype>

// TypeDefs
typedef std::vector<std::string> vstring_t;
typedef std::vector<double_t>    vdouble_t;
typedef std::string              string_t;
typedef int16_t                  value_t;

namespace eqnibbler {

struct token {
    value_t  type;
    string_t content;
    double_t value;
};

class Math {

public:

   /**
    * Constructor/Destructor
    */

    Math() {};
    ~Math() {};

   /**
    * Setters/Getters
    */

    bool setVariables(vstring_t);
    bool setEquation(string_t);

   /**
    * Methods
    */

    bool Eval(vdouble_t, double_t*);

   /**
    * Properties
    */

private:

   /**
    * Member Functions
    */

    bool    eqLexer();
    bool    eqParser();

    value_t validOperator(string_t*);
    value_t validUnary(string_t*);
    value_t validWord(string_t*);
    value_t validNumber(string_t*, double_t*);
    value_t validSeparator(string_t*);

    void    precedenceLogical(string_t, int32_t*, int32_t*);
    void    precedenceMath(string_t, bool, int32_t*, int32_t*);

    bool    evalVariable(string_t, vdouble_t*, double_t*);
    bool    evalConstant(string_t, double_t*);
    bool    evalFunction(string_t, vdouble_t*, double_t*);
    bool    evalLogical(string_t, vdouble_t*, double_t*);
    bool    evalMath(string_t, bool, vdouble_t*, double_t*);

   /**
    * Member Variables
    */

    bool               m_Parsed    = false;

    const vstring_t    m_WFunc     = {"sin","cos","tan","exp","log","abs","mod","if"};
    const vstring_t    m_WConst    = {"pi"};
    const vstring_t    m_OLogical  = {"&&","||","==","<",">",">=","<=","!=","<>"};
    const vstring_t    m_OMath     = {"+","-","*","/","^","_"};
    const vstring_t    m_OUnary    = {"+","-"};

    string_t           m_Equation;
    vstring_t          m_WVariable;
    std::vector<token> m_Tokens;
    std::vector<token> m_ParseTree;

};

} // End NameSpace

#endif
