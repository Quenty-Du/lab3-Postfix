#ifndef __VALIDATOR_H__
#define __VALIDATOR_H__

#include <stdexcept>


#include "parser.h"
#include "tstack.h"


enum class Validate {
    START, ARG, FUNC, PAREN
};


// Пространство имен для функций валидации
class Validator {
    static void validateOperands(const TVector<Token>& token, size_t pos);

    static bool validateNumbers(const TVector<Token>& token, size_t pos);
    static bool validateBinaryOperators(const TVector<Token>& token, size_t pos);
    static bool validateBrackets(const TVector<Token>& token, size_t pos, TStack<Validate>& bracket_check, TStack<Validate>& argument_check, bool& expect_func_args);
    static bool validateFunctions(const TVector<Token>& token, size_t pos, TStack<Validate>& argument_check, bool& expect_func_args);
    static bool validateCommas(const TVector<Token>& token, size_t pos, TStack<Validate>& argument_check);
    static bool validateUnaryOperators(const TVector<Token>& token, size_t pos);
    
    static void validateDots(const TVector<Token>& token, size_t pos);  
public:
    static void validation(const TVector<Token>& infix_tokens);
};


#endif