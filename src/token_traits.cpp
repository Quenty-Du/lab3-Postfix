#include "../include/token_traits.h"
#include "../include/parser.h"


bool TokenTraits::isOperand(TokenType type) {
    return type == TokenType::NUMBER ||
           type == TokenType::VARIABLE ||
           type == TokenType::CONSTANT;
}


bool TokenTraits::isFunction(TokenType type) {
    return (type >= TokenType::SQRT && type <= TokenType::FACT);
}


bool TokenTraits::canStartExpr(TokenType type) {
    return isOperand(type) ||
           isFunction(type) ||
           type == TokenType::LEFT_PAREN;
}


bool TokenTraits::canEndExpr(TokenType type) {
    return TokenTraits::isOperand(type) ||
           type == TokenType::RIGHT_PAREN;
}


bool TokenTraits::isOperator(TokenType type) {
    return type >= TokenType::PLUS && type <= TokenType::UNARY_PLUS;
}


size_t TokenTraits::getPriority(TokenType type) {
    switch (type) {
        case TokenType::UNARY_PLUS:
        case TokenType::UNARY_MINUS:
            return 4;
        case TokenType::MUL:
        case TokenType::DIV:
            return 3;
        case TokenType::PLUS:
        case TokenType::MINUS:
            return 2;
        default:
            return 0;
    }
}