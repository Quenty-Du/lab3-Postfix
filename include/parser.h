#ifndef __PARSER_H__
#define __PARSER_H__

#include "tvector.h"


std::string intToString(int val);


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

    // Методы для определения типа токена
    static bool isSpace(char c) noexcept;
    static bool isDigit(char c) noexcept;
    static bool isLetter(char c) noexcept;
    static bool isDot(char c) noexcept;
    static bool isParen(char c) noexcept;
    static bool isBinaryOperator(char c) noexcept;
    static bool isComma(char c) noexcept;
    bool isUnary(size_t token_count) noexcept;

    // Методы для выделения токенов из строки
    static std::string extractNumber(const std::string& expr, size_t& pos);
    static std::string extractWord(const std::string& expr, size_t& pos);

    // Методы для создания токенов
    static std::string toLower(const std::string& input_word);
    static TokenType getTokenType(const std::string& lexeme) noexcept;

    void parse(const std::string& expr); // Разбиение выражения на токены
};

#endif