/**
 *  Equation Nibbler Library
 * ==========================
 *  A math function parser using a reverse polish notation algorithm.
 *  Loosely based on: http://www.codeproject.com/Articles/345888/How-to-write-a-simple-interpreter-in-JavaScript
 * 
 *  ToDo: Merge precedence of math and logical. See:
 *        https://www.bouraspage.com/repository/algorithmic-thinking/what-is-the-order-of-precedence-of-arithmetic-comparison-and-logical-operators
 */

#include "clsMath.hpp"

using namespace std;
using namespace smath;

// ****************************************************************************************************************************** //

/**
 *  Method :: setVariables
 * ========================
 */

bool Math::setVariables(vstring_t vsVariable) {

    bool isReserved = false;

    for(auto sItem : vsVariable) {
        if( sItem == "pi"   || sItem == "sin"   || sItem == "cos"  ||
            sItem == "tan"  || sItem == "asin"  || sItem == "acos" ||
            sItem == "atan" || sItem == "atan2" || sItem == "exp"  ||
            sItem == "log"  || sItem == "abs"   || sItem == "mod"  ) {
            isReserved = true;
        }
    }
    if(!isReserved) m_WVariable = vsVariable;

    return !isReserved;
}

// ****************************************************************************************************************************** //

/**
 *  Method :: setEquation
 * =======================
 *  Sets the equation to be parsed
 */

bool Math::setEquation(string_t sEquation) {

    // Append a space to make sure last character is evaluated
    m_Equation = sEquation + " ";

    bool okLexer  = eqLexer();
    if(!okLexer) return false;

    bool okParser = eqParser();
    if(!okParser) return false;

    m_Parsed = true;
    return true;
}

// ****************************************************************************************************************************** //

/**
 *  Method :: Eval
 * ================
 *  Evaluate the Parsed Function
 *  Takes vector of variables and vector of values as input
 *  Using Reverse Polish notation
 *  https://en.wikipedia.org/wiki/Reverse_Polish_notation
 */

bool Math::Eval(vdouble_t vdValues, double_t* pReturn) {

    vdouble_t vdStack;
    size_t    iPos;
    bool      valFound;

    double_t  dVal;
    double_t  dValL;
    double_t  dValR;

#ifdef DEBUG
    printf("DEBUG> Evaluating Equation\n");
    for(size_t i=0; i<m_WVariable.size(); i++) {
        printf("DEBUG>  * %-5s = %10.3e\n", m_WVariable[i].c_str(), vdValues[i]);
    }
    printf("DEBUG> Computing\n");
#endif

    if(!m_Parsed) {
        printf("Math Eval Error: No valid equation to evaluate\n");
        return false;
    }

    if(m_WVariable.size() != vdValues.size()) {
        printf("Math Eval Error: Values vector must be the same length as variables vector\n");
        return false;
    }

    for(auto tItem : m_ParseTree) {

        if(tItem.size == 0) {
            if(tItem.eval == EVAL_NUMBER) {
                vdStack.push_back(tItem.value);
            } else
            if(tItem.eval == EVAL_VARIABLE) {
                iPos     = 0;
                valFound = false;
                for(auto sItem : m_WVariable) {
                    if(sItem == tItem.content) {
                        vdStack.push_back(vdValues.at(iPos));
                        valFound = true;
                        break;
                    }
                    iPos++;
                }
                if(!valFound) {
                    printf("Math Eval Error: Unknown variable %s\n",tItem.content.c_str());
                    return false;
                }
            } else
            if(tItem.eval == EVAL_END) {
                break;
            } else {
                printf("Math Eval Error: Unknown error in size = 0, content = '%s'\n", tItem.content.c_str());
                return false;
            }
        } else
        if(tItem.size == 1) {
            dVal = vdStack.back(); vdStack.pop_back();
            if(tItem.eval == EVAL_UNARY_PLUS) {
                vdStack.push_back(dVal);
            } else
            if(tItem.eval == EVAL_UNARY_MINUS) {
                vdStack.push_back(-dVal);
            } else
            if(tItem.eval == EVAL_FUNC_SIN) {
                vdStack.push_back(sin(dVal));
            } else
            if(tItem.eval == EVAL_FUNC_COS) {
                vdStack.push_back(cos(dVal));
            } else
            if(tItem.eval == EVAL_FUNC_TAN) {
                vdStack.push_back(tan(dVal));
            } else
            if(tItem.eval == EVAL_FUNC_ASIN) {
                vdStack.push_back(asin(dVal));
            } else
            if(tItem.eval == EVAL_FUNC_ACOS) {
                vdStack.push_back(acos(dVal));
            } else
            if(tItem.eval == EVAL_FUNC_ATAN) {
                vdStack.push_back(atan(dVal));
            } else
            if(tItem.eval == EVAL_FUNC_EXP) {
                vdStack.push_back(exp(dVal));
            } else
            if(tItem.eval == EVAL_FUNC_LOG) {
                vdStack.push_back(log(dVal));
            } else
            if(tItem.eval == EVAL_FUNC_ABS) {
                vdStack.push_back(abs(dVal));
            } else {
                printf("Math Eval Error: Unknown error in size = 1, content = '%s'\n", tItem.content.c_str());
                return false;
            }
        }
        if(tItem.size == 2) {
            dValR = vdStack.back(); vdStack.pop_back();
            dValL = vdStack.back(); vdStack.pop_back();
            if(tItem.eval == EVAL_MATH_PLUS) {
                vdStack.push_back(dValL + dValR);
            } else
            if(tItem.eval == EVAL_MATH_MINUS) {
                vdStack.push_back(dValL - dValR);
            } else
            if(tItem.eval == EVAL_MATH_MULT) {
                vdStack.push_back(dValL * dValR);
            } else
            if(tItem.eval == EVAL_MATH_DIV) {
                vdStack.push_back(dValL / dValR);
            } else
            if(tItem.eval == EVAL_MATH_POW) {
                vdStack.push_back(pow(dValL,dValR));
            } else
            if(tItem.eval == EVAL_FUNC_ATAN2) {
                vdStack.push_back(atan2(dValL,dValR));
            } else
            if(tItem.eval == EVAL_FUNC_MOD) {
                if(dValL == floor(dValL) && dValR == floor(dValR)) {
                    vdStack.push_back((int)floor(dValL)%(int)floor(dValR));
                } else {
                    printf("Math Eval Error: Function mod() requires integer values\n");
                    return false;
                }
            } else
            if(tItem.eval == EVAL_LOGICAL_AND) {
                if(dValL && dValR) {
                    vdStack.push_back(EVAL_TRUE);
                } else {
                    vdStack.push_back(EVAL_FALSE);
                }
            } else
            if(tItem.eval == EVAL_LOGICAL_OR) {
                if(dValL || dValR) {
                    vdStack.push_back(EVAL_TRUE);
                } else {
                    vdStack.push_back(EVAL_FALSE);
                }
            } else
            if(tItem.eval == EVAL_LOGICAL_EQ) {
                if(dValL == dValR) {
                    vdStack.push_back(EVAL_TRUE);
                } else {
                    vdStack.push_back(EVAL_FALSE);
                }
            } else
            if(tItem.eval == EVAL_LOGICAL_NE) {
                if(dValL != dValR) {
                    vdStack.push_back(EVAL_TRUE);
                } else {
                    vdStack.push_back(EVAL_FALSE);
                }
            } else
            if(tItem.eval == EVAL_LOGICAL_LT) {
                if(dValL < dValR) {
                    vdStack.push_back(EVAL_TRUE);
                } else {
                    vdStack.push_back(EVAL_FALSE);
                }
            } else
            if(tItem.eval == EVAL_LOGICAL_GT) {
                if(dValL > dValR) {
                    vdStack.push_back(EVAL_TRUE);
                } else {
                    vdStack.push_back(EVAL_FALSE);
                }
            } else
            if(tItem.eval == EVAL_LOGICAL_LE) {
                if(dValL <= dValR) {
                    vdStack.push_back(EVAL_TRUE);
                } else {
                    vdStack.push_back(EVAL_FALSE);
                }
            } else
            if(tItem.eval == EVAL_LOGICAL_GE) {
                if(dValL >= dValR) {
                    vdStack.push_back(EVAL_TRUE);
                } else {
                    vdStack.push_back(EVAL_FALSE);
                }
            } else {
                printf("Math Eval Error: Unknown error in size = 2, content = '%s'\n", tItem.content.c_str());
                return false;
            }
        } else
        if(tItem.size == 3) {
            dValR = vdStack.back(); vdStack.pop_back();
            dValL = vdStack.back(); vdStack.pop_back();
            dVal  = vdStack.back(); vdStack.pop_back();
            if(tItem.eval == EVAL_SPECIAL_IF) {
                if(dVal != EVAL_FALSE) {
                    vdStack.push_back(dValL);
                } else {
                    vdStack.push_back(dValR);
                }
            } else {
                printf("Math Eval Error: Unknown error in size = 3, content = '%s'\n", tItem.content.c_str());
                return false;
            }
        }

#ifdef DEBUG
        printf("DEBUG>  * Stack: ");
        for(auto dValue : vdStack) {
            printf("%10.3e | ", dValue);
        }
        printf("<< '%s'\n", tItem.content.c_str());
#endif
    }

    *pReturn = vdStack.front();

    return true;
}

// ****************************************************************************************************************************** //

/**
 *  Function :: eqLexer
 * =====================
 *  Builds the equation lexer
 */

bool Math::eqLexer() {

    value_t       idCurr;
    value_t       idPrev  = MT_NONE;
    char          cPrev   = '#';
    string_t      sBuffer = "";
    vector<token> vTokens;

#ifdef DEBUG
    printf("DEBUG> This is eqLexer\n");
    printf("DEBUG>  * Equation: '%s'\n", m_Equation.c_str());
#endif

    for(char cCurr : m_Equation) {

        // Get specific type
        idCurr = MT_NONE;
        if(isdigit(cCurr)) idCurr = MT_NUMBER;
        if(isalpha(cCurr)) idCurr = MT_WORD;
        if(ispunct(cCurr)) idCurr = MT_OPERATOR;

        // Numbers following characters are part of words
        if(idCurr == MT_NUMBER && idPrev == MT_WORD) idCurr = MT_WORD;
        // '.' is always part of a number
        if(cCurr == '.') idCurr = MT_NUMBER;
        // 'd' or 'e' following a number denotes exponent
        if((cCurr == 'd' || cCurr == 'e') && idPrev == MT_NUMBER) {
            idCurr = MT_NUMBER;
        }
        // '-' after an 'e' or 'd' that is a number, is also a part of a number
        if((cCurr == '-' || cCurr == '+') && (cPrev == 'd' || cPrev == 'e') && idPrev == MT_NUMBER) {
            idCurr = MT_NUMBER;
        }
        // Check if operator is actually a separataor
        if(cCurr == '(' || cCurr == ')' || cCurr == ',') {
            idCurr = MT_SEPARATOR;
        }
        // Check if unary minus
        if((cCurr == '-' || cCurr == '+') && !(idPrev == MT_NUMBER || idPrev == MT_WORD || (idPrev == MT_NONE && cPrev != '#'))) {
            idCurr = MT_UNARYOP;
        }

        // If a new type was encountered, push the previous onto the lexer
        if(idCurr != idPrev || idPrev == MT_SEPARATOR) {
            if(idPrev != MT_NONE) {
                vTokens.push_back(token({idPrev, sBuffer, 0.0, 0, 0}));
            }
            sBuffer = "";
        }
        sBuffer += cCurr;

        // Set previous values for next loop
        idPrev = idCurr;
        cPrev  = cCurr;
    }

    // Clean up lexer and check for invalid entries
    idPrev = MP_NONE;
    for(auto tItem : vTokens) {

        value_t  idType = MP_NONE;
        value_t  idEval = EVAL_NONE;
        value_t  nParms = 0;
        double_t dValue = 0.0;

        switch(tItem.type) {
        case MT_OPERATOR:
            if(tItem.content == "+") {
                idType = MP_MATH;
                idEval = EVAL_MATH_PLUS;
                nParms = 2;
            } else
            if(tItem.content == "-") {
                idType = MP_MATH;
                idEval = EVAL_MATH_MINUS;
                nParms = 2;
            } else
            if(tItem.content == "*") {
                idType = MP_MATH;
                idEval = EVAL_MATH_MULT;
                nParms = 2;
            } else
            if(tItem.content == "/") {
                idType = MP_MATH;
                idEval = EVAL_MATH_DIV;
                nParms = 2;
            } else
            if(tItem.content == "^") {
                idType = MP_MATH;
                idEval = EVAL_MATH_POW;
                nParms = 2;
            } else
            if(tItem.content == "&&") {
                idType = MP_LOGICAL;
                idEval = EVAL_LOGICAL_AND;
                nParms = 2;
            } else
            if(tItem.content == "||") {
                idType = MP_LOGICAL;
                idEval = EVAL_LOGICAL_OR;
                nParms = 2;
            } else
            if(tItem.content == "==") {
                idType = MP_LOGICAL;
                idEval = EVAL_LOGICAL_EQ;
                nParms = 2;
            } else
            if(tItem.content == "!=") {
                idType = MP_LOGICAL;
                idEval = EVAL_LOGICAL_NE;
                nParms = 2;
            } else
            if(tItem.content == "<") {
                idType = MP_LOGICAL;
                idEval = EVAL_LOGICAL_LT;
                nParms = 2;
            } else
            if(tItem.content == ">") {
                idType = MP_LOGICAL;
                idEval = EVAL_LOGICAL_GT;
                nParms = 2;
            } else
            if(tItem.content == "<=") {
                idType = MP_LOGICAL;
                idEval = EVAL_LOGICAL_LE;
                nParms = 2;
            } else
            if(tItem.content == ">=") {
                idType = MP_LOGICAL;
                idEval = EVAL_LOGICAL_GE;
                nParms = 2;
            } else {
                idType = MP_INVALID;
                idEval = EVAL_NONE;
                nParms = 0;
            }
            break;

        case MT_UNARYOP:
            if(tItem.content == "+") {
                idType = MP_UNARY;
                idEval = EVAL_UNARY_PLUS;
                nParms = 1;
            } else
            if(tItem.content == "-") {
                idType = MP_UNARY;
                idEval = EVAL_UNARY_MINUS;
                nParms = 1;
            } else {
                idType = MP_INVALID;
                idEval = EVAL_NONE;
                nParms = 0;
            }
            break;

        case MT_NUMBER:
            try {
                dValue = stof(tItem.content);
                idType = MP_NUMBER;
                idEval = EVAL_NUMBER;
                nParms = 0;
            } catch(...) {
                idType = MP_INVALID;
                idEval = EVAL_NONE;
                nParms = 0;
            }
            break;

        case MT_WORD:
            if(tItem.content == "sin") {
                idType = MP_FUNC;
                idEval = EVAL_FUNC_SIN;
                nParms = 1;
            } else
            if(tItem.content == "cos") {
                idType = MP_FUNC;
                idEval = EVAL_FUNC_COS;
                nParms = 1;
            } else
            if(tItem.content == "tan") {
                idType = MP_FUNC;
                idEval = EVAL_FUNC_TAN;
                nParms = 1;
            } else
            if(tItem.content == "asin") {
                idType = MP_FUNC;
                idEval = EVAL_FUNC_ASIN;
                nParms = 1;
            } else
            if(tItem.content == "acos") {
                idType = MP_FUNC;
                idEval = EVAL_FUNC_ACOS;
                nParms = 1;
            } else
            if(tItem.content == "atan") {
                idType = MP_FUNC;
                idEval = EVAL_FUNC_ATAN;
                nParms = 1;
            } else
            if(tItem.content == "atan2") {
                idType = MP_FUNC;
                idEval = EVAL_FUNC_ATAN2;
                nParms = 2;
            } else
            if(tItem.content == "exp") {
                idType = MP_FUNC;
                idEval = EVAL_FUNC_EXP;
                nParms = 1;
            } else
            if(tItem.content == "log") {
                idType = MP_FUNC;
                idEval = EVAL_FUNC_LOG;
                nParms = 1;
            } else
            if(tItem.content == "abs") {
                idType = MP_FUNC;
                idEval = EVAL_FUNC_ABS;
                nParms = 1;
            } else
            if(tItem.content == "mod") {
                idType = MP_FUNC;
                idEval = EVAL_FUNC_MOD;
                nParms = 2;
            } else
            if(tItem.content == "if") {
                idType = MP_FUNC;
                idEval = EVAL_SPECIAL_IF;
                nParms = 3;
            } else
            if(tItem.content == "pi") {
                idType = MP_CONST;
                idEval = EVAL_NUMBER;
                nParms = 0;
                dValue = M_PI;
            } else {
                for(string_t sItem : m_WVariable) {
                    if(sItem == tItem.content) {
                        idType = MP_VARIABLE;
                        idEval = EVAL_VARIABLE;
                        nParms = 0;
                    }
                }
                if(idType == MP_NONE) {
                    idType = MP_INVALID;
                    idEval = EVAL_NONE;
                    nParms = 0;
                }
            }
            break;

        case MT_SEPARATOR:
            if(tItem.content == "(") {
                idType = MP_LBRACK;
                idEval = EVAL_NONE;
                nParms = 0;
            } else
            if(tItem.content == ")") {
                idType = MP_RBRACK;
                idEval = EVAL_NONE;
                nParms = 0;
            } else
            if(tItem.content == ",") {
                idType = MP_COMMA;
                idEval = EVAL_NONE;
                nParms = 0;
            } else {
                idType = MP_INVALID;
                idEval = EVAL_NONE;
                nParms = 0;
            }
            break;
        }

        if(idType == MP_INVALID || (idType == idPrev && !(idType == MP_LBRACK || idType == MP_RBRACK ))) {
            printf("Math Error: Cannot parse token '%s'\n", tItem.content.c_str());
            return false;
        } else {
            m_Tokens.push_back(token({idType, tItem.content, dValue, idEval, nParms}));
        }

        idPrev = idType;
    }

    // Check that we actually have something
    if(m_Tokens.size() == 0) {
        printf("Math Error: No equation\n");
        return false;
    }

    // Add end token
    m_Tokens.push_back(token({MP_END, "end", 0.0, EVAL_END, 0}));

#ifdef DEBUG
    // Echo lexer for debug
    printf("DEBUG> Lexer result:\n");
    size_t lIdx = 0;
    for(auto tItem : m_Tokens) {
        lIdx++;
        printf("DEBUG>  * Item %2d : ", int(lIdx));
        printf("Type = %2d, ",          tItem.eval);
        printf("Size = %1d, ",          tItem.size);
        printf("Value = %23.16e, ",     tItem.value);
        printf("Content = '%s'\n",      tItem.content.c_str());
    }
#endif

    return true;
}

// ****************************************************************************************************************************** //

/**
 *  Function :: eqParser
 * ======================
 *  Parses the equation using the Shunting-yard algorithm
 *  Based on https://en.wikipedia.org/wiki/Shunting-yard_algorithm
 */

bool Math::eqParser() {

    vector<token> vtOutput;
    vector<token> vtStack;

#ifdef DEBUG
    printf("DEBUG> This is eqParser using Shunting-Yard algorithm\n");
    uint32_t nStep = 0;
#endif

    for(auto tItem : m_Tokens) {

        int  itemPrec   = 0;
        int  itemAssoc  = 0;
        int  stackPrec  = 0;
        int  stackAssoc = 0;
        int  iErase     = 0;
        bool isClosed   = false;

        switch(tItem.type) {

        case MP_NUMBER:
            vtOutput.push_back(tItem);
            break;

        case MP_VARIABLE:
            vtOutput.push_back(tItem);
            break;

        case MP_CONST:
            vtOutput.push_back(tItem);
            break;

        case MP_FUNC:
            vtStack.insert(vtStack.begin(), tItem);
            break;

        case MP_LOGICAL:
            iErase = 0;

            precedenceLogical(tItem.content,&itemPrec,&itemAssoc);
            for(auto tStack : vtStack) {
                precedenceLogical(tStack.content,&stackPrec,&stackAssoc);
                if( tStack.type == MP_MATH &&
                    ( (itemAssoc == ASSOC_L && itemPrec <= stackPrec) ||
                        (itemAssoc == ASSOC_R && itemPrec <  stackPrec) ) ) {
                    vtOutput.push_back(tStack);
                    iErase++;
                } else {
                    break;
                }
            }

            if(iErase > 0) {
                vtStack.erase(vtStack.begin(),vtStack.begin()+iErase);
            }
            vtStack.insert(vtStack.begin(), tItem);

            break;

        case MP_MATH:
            iErase = 0;

            precedenceMath(tItem.content,false,&itemPrec,&itemAssoc);
            for(auto tStack : vtStack) {
                precedenceMath(tStack.content,false,&stackPrec,&stackAssoc);
                if( (tStack.type == MP_MATH || tStack.type == MP_UNARY) &&
                    ( (itemAssoc == ASSOC_L && itemPrec <= stackPrec) ||
                        (itemAssoc == ASSOC_R && itemPrec <  stackPrec) ) ) {
                    vtOutput.push_back(tStack);
                    iErase++;
                } else {
                    break;
                }
            }

            if(iErase > 0) {
                vtStack.erase(vtStack.begin(),vtStack.begin()+iErase);
            }
            vtStack.insert(vtStack.begin(), tItem);

            break;

        case MP_UNARY:
            iErase = 0;

            precedenceMath(tItem.content,true,&itemPrec,&itemAssoc);
            for(auto tStack : vtStack) {
                precedenceMath(tStack.content,true,&stackPrec,&stackAssoc);
                if( (tStack.type == MP_MATH || tStack.type == MP_UNARY) &&
                    ( (itemAssoc == ASSOC_L && itemPrec <= stackPrec) ||
                        (itemAssoc == ASSOC_R && itemPrec <  stackPrec) ) ) {
                    vtOutput.push_back(tStack);
                    iErase++;
                } else {
                    break;
                }
            }

            if(iErase > 0) {
                vtStack.erase(vtStack.begin(),vtStack.begin()+iErase);
            }
            vtStack.insert(vtStack.begin(), tItem);

            break;

        case MP_LBRACK:
            vtStack.insert(vtStack.begin(), tItem);
            break;

        case MP_RBRACK:
            iErase   = 0;
            isClosed = false;

            for(auto tStack : vtStack) {
                if(tStack.type != MP_LBRACK) {
                    vtOutput.push_back(tStack);
                    iErase++;
                } else {
                    isClosed = true;
                    break;
                }
            }

            if(iErase > 0) {
                vtStack.erase(vtStack.begin(),vtStack.begin()+iErase);
            }

            // Check if brackets were closed
            if(isClosed) {
                vtStack.erase(vtStack.begin());
            } else {
                printf("Math Error: Paranthesis mismatch\n");
                return false;
            }

            // Check if the next token on stack is a function
            if(vtStack.front().type == MP_FUNC) {
                vtOutput.push_back(vtStack.front());
                vtStack.erase(vtStack.begin());
            }

            break;

        case MP_COMMA:
            iErase   = 0;
            isClosed = false;

            for(auto tStack : vtStack) {
                if(tStack.type != MP_LBRACK) {
                    vtOutput.push_back(tStack);
                    iErase++;
                } else {
                    isClosed = true;
                    break;
                }
            }

            if(iErase > 0) {
                vtStack.erase(vtStack.begin(),vtStack.begin()+iErase);
            }

            break;

        case MP_END:
            iErase = 0;

            if(vtStack.size() > 0) {
                if( vtStack.front().type == MP_LBRACK ||
                    vtStack.front().type == MP_RBRACK ) {
                printf("Math Error: Paranthesis mismatch\n");
                return false;
                }

                for(auto tStack : vtStack) {
                    vtOutput.push_back(tStack);
                    iErase++;
                }
                if(iErase > 0) {
                    vtStack.erase(vtStack.begin(),vtStack.begin()+iErase);
                }
            }

            vtOutput.push_back(tItem);
            m_ParseTree = vtOutput;

#ifdef DEBUG
            nStep++;
            printf("DEBUG> Step %d\n", nStep);
            printf("DEBUG>  * Current : %s\n", tItem.content.c_str());
            printf("DEBUG>  * Stack   : ");
            for(auto tTemp : vtStack) {
                printf("%s  ",tTemp.content.c_str());
            }
            printf("\n");
            printf("DEBUG>  * Output  : ");
            for(auto tTemp : vtOutput) {
                printf("%s  ",tTemp.content.c_str());
            }
            printf("\n");
#endif

            return true;
            break;
        }

#ifdef DEBUG
        nStep++;
        printf("DEBUG> Step %d\n", nStep);
        printf("DEBUG>  * Current : %s\n", tItem.content.c_str());
        printf("DEBUG>  * Stack   : ");
        for(auto tTemp : vtStack) {
            printf("%s  ",tTemp.content.c_str());
        }
        printf("\n");
        printf("DEBUG>  * Output  : ");
        for(auto tTemp : vtOutput) {
            printf("%s  ",tTemp.content.c_str());
        }
        printf("\n");
#endif
    }

    return false;
}

// ****************************************************************************************************************************** //

/**
 *  Function :: precedenceLogical
 * ===============================
 *  Returns precedence and associativity of operator
 */

void Math::precedenceLogical(string_t sOperator, int32_t* pPrecedence, int32_t* pAssoc) {

    if(sOperator == "&&") {
        *pPrecedence = 3;
        *pAssoc = ASSOC_L;
    } else
    if(sOperator == "||") {
        *pPrecedence = 2;
        *pAssoc = ASSOC_L;
    } else
    if(sOperator == "==") {
        *pPrecedence = 9;
        *pAssoc = ASSOC_L;
    } else
    if(sOperator == "<") {
        *pPrecedence = 9;
        *pAssoc = ASSOC_L;
    } else
    if(sOperator == ">") {
        *pPrecedence = 9;
        *pAssoc = ASSOC_L;
    } else
    if(sOperator == ">=") {
        *pPrecedence = 9;
        *pAssoc = ASSOC_L;
    } else
    if(sOperator == "<=") {
        *pPrecedence = 9;
        *pAssoc = ASSOC_L;
    } else
    if(sOperator == "!=") {
        *pPrecedence = 9;
        *pAssoc = ASSOC_L;
    } else
    if(sOperator == "<>") {
        *pPrecedence = 9;
        *pAssoc = ASSOC_L;
    }

    return;
}

// ****************************************************************************************************************************** //

/**
 *  Function :: precedenceMath
 * ============================
 *  Returns precedence and associativity of operator
 */

void Math::precedenceMath(string_t sOperator, bool isUnary, int32_t* pPrecedence, int32_t* pAssoc) {

    if(sOperator == "^") {
        *pPrecedence = 5;
        *pAssoc = ASSOC_R;
    } else
    if(sOperator == "*") {
        *pPrecedence = 3;
        *pAssoc = ASSOC_L;
    } else
    if(sOperator == "/") {
        *pPrecedence = 3;
        *pAssoc = ASSOC_L;
    } else
    if(sOperator == "+") {
        if(isUnary) {
            *pPrecedence = 4;
            *pAssoc = ASSOC_R;
        } else {
            *pPrecedence = 2;
            *pAssoc = ASSOC_L;
        }
    } else
    if(sOperator == "-") {
        if(isUnary) {
            *pPrecedence = 4;
            *pAssoc = ASSOC_R;
        } else {
            *pPrecedence = 2;
            *pAssoc = ASSOC_L;
        }
    }

    return;
}

// ****************************************************************************************************************************** //

// End Class Math
