#ifndef __PARSER_H__
#define __PARSER_H__

#include "tvector.h"
#include "tstack.h"


enum class TokenType {
    // Операнды
    NUMBER, VARIABLE, CONSTANT,

    // Бинарные операторы
    PLUS, MINUS, MUL, DIV,

    // Унарные операторы
    UNARY_MINUS, UNARY_PLUS,

    // Функции
    SQRT, POW, LOG,
    SIN, COS, TG, CTG, ASIN, ACOS, ATG, ACTG,
    SH, CH, TH, CTH, ASH, ACH, ATH, ACTH,
    FACT,

    // Разделители
    LEFT_PAREN, RIGHT_PAREN, COMMA, DOT,

    EMPTY
};


// Подструктура хранения токенов
struct Token {
    TokenType type;
    std::string value;
    Token(TokenType t = TokenType::EMPTY, const std::string& val = "");
};


class Parser {
    TVector<Token> infix_tokens; // Считанная строка токенов
    TVector<Token> postfix_tokens; // Переведенная в постфиксную форму строка токенов
    bool parsed = false;

    // Методы для определения типа токена
    bool isUnary(size_t token_count) noexcept;

    // Методы для создания токенов
    static TokenType getTokenType(const std::string& lexeme) noexcept;

    void parse(const std::string& expr); // Разбиение выражения на токены
    void toPostfix();

public:
    const TVector<Token>& getPostfix(const std::string& expr);
};

#endif