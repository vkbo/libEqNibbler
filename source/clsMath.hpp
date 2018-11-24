/**
 *  Equation Nibbler Library
 * ==========================
 *  A math function parser using a reverse polish notation algorithm.
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

#define EVAL_NONE        -1
#define EVAL_NUMBER       1
#define EVAL_VARIABLE     2
#define EVAL_CONST_PI     3
#define EVAL_UNARY_PLUS   4
#define EVAL_UNARY_MINUS  5
#define EVAL_MATH_PLUS    6
#define EVAL_MATH_MINUS   7
#define EVAL_MATH_MULT    8
#define EVAL_MATH_DIV     9
#define EVAL_MATH_POW     10
#define EVAL_LOGICAL_AND  11
#define EVAL_LOGICAL_OR   12
#define EVAL_LOGICAL_EQ   13
#define EVAL_LOGICAL_NE   14
#define EVAL_LOGICAL_LT   15
#define EVAL_LOGICAL_GT   16
#define EVAL_LOGICAL_LE   17
#define EVAL_LOGICAL_GE   18
#define EVAL_FUNC_SIN     19
#define EVAL_FUNC_COS     20
#define EVAL_FUNC_TAN     21
#define EVAL_FUNC_ASIN    22
#define EVAL_FUNC_ACOS    23
#define EVAL_FUNC_ATAN    24
#define EVAL_FUNC_ATAN2   25
#define EVAL_FUNC_EXP     26
#define EVAL_FUNC_LOG     27
#define EVAL_FUNC_ABS     28
#define EVAL_FUNC_MOD     29
#define EVAL_SPECIAL_IF   30
#define EVAL_END          31

// Includes
#include <iostream>
#include <cmath>
#include <vector>

// TypeDefs
typedef std::vector<std::string> vstring_t;
typedef std::vector<double_t>    vdouble_t;
typedef std::string              string_t;
typedef int32_t                  value_t;

namespace smath {

struct token {
    value_t  type;
    string_t content;
    double_t value;
    value_t  eval;
    value_t  size;
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

    value_t validWord(string_t*);

    void    precedenceLogical(string_t, int32_t*, int32_t*);
    void    precedenceMath(string_t, bool, int32_t*, int32_t*);

   /**
    * Member Variables
    */

    bool               m_Parsed    = false;

    string_t           m_Equation;
    vstring_t          m_WVariable;
    std::vector<token> m_Tokens;
    std::vector<token> m_ParseTree;

};

} // End NameSpace

#endif
