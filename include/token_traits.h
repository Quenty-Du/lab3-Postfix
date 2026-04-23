#ifndef __TOKEN_TRAITS_H__
#define __TOKEN_TRAITS_H__


#include "parser.h"


namespace TokenTraits {
    bool isOperand(TokenType type);
    bool isFunction(TokenType type);
    bool canStartExpr(TokenType type);
    bool canEndExpr(TokenType type);
    bool isOperator(TokenType type);

    size_t getPriority(TokenType type);
}


#endif