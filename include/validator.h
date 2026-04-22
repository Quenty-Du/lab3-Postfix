#ifndef __VALIDATOR_H__
#define __VALIDATOR_H__

#include <stdexcept>


#include "parser.h"
#include "tstack.h"


enum class Validate {
    START, ARG, FUNC, PAREN
};


// Пространство имен для функций валидации
namespace Validator {
    bool isOperand(TokenType type);
    bool isFunction(TokenType type);
    bool canStartExpr(TokenType type);
    bool canEndExpr(TokenType type);

    void validation(const TVector<Token>& infix_form);

    void validateOperands(const TVector<Token>& token, size_t pos);

    bool validateBinaryOperators(const TVector<Token>& token, size_t pos);
    bool validateBrackets(const TVector<Token>& token, size_t pos, TStack<Validate>& bracket_check, TStack<Validate>& argument_check, bool& expect_func_args);
    bool validateFunctions(const TVector<Token>& token, size_t pos, TStack<Validate>& argument_check, bool& expect_func_args);
    bool validateCommas(const TVector<Token>& token, size_t pos, TStack<Validate>& argument_check);
    bool validateUnaryOperators(const TVector<Token>& token, size_t pos);
    
    void validateDots(const TVector<Token>& token, size_t pos);  
}


#endif