#include "../include/parser.h"
#include "../include/validator.h"
#include "../include/token_traits.h"
#include "../include/string_lib.h"


Token::Token(TokenType t, const std::string& val)
    : type(t), value(val) {
}


bool Parser::isUnary(size_t token_count) noexcept {
    return (token_count == 0 || infix_tokens[token_count - 1].type == TokenType::LEFT_PAREN);
}


TokenType Parser::getTokenType(const std::string& lexeme) noexcept {
    static const char* function_names[] = {
        "sqrt", "pow", "log",
        "sin", "cos", "tg", "ctg", "asin", "acos", "atg", "actg",
        "sh", "ch", "th", "cth", "ash", "ach", "ath", "acth",
        "fact"
    };

    static const TokenType function_types[] = {
        TokenType::SQRT, TokenType::POW, TokenType::LOG,
        TokenType::SIN, TokenType::COS, TokenType::TG, TokenType::CTG, TokenType::ASIN, TokenType::ACOS, TokenType::ATG, TokenType::ACTG,
        TokenType::SH, TokenType::CH, TokenType::TH, TokenType::CTH, TokenType::ASH, TokenType::ACH, TokenType::ATH, TokenType::ACTH,
        TokenType::FACT
    };

    const size_t func_count = sizeof(function_names) / sizeof(function_names[0]);

    for (size_t i = 0; i < func_count; ++i) {
        if (lexeme == function_names[i]) {
            return function_types[i];
        }
    }

    // Остаточная проверка на константы (их 2 штуки)
    if (lexeme == "pi" || lexeme == "e") {
        return TokenType::CONSTANT;
    }

    return TokenType::VARIABLE;
}


void Parser::parse(const std::string& expr) {
    const size_t expr_len = expr.length();
    infix_tokens = TVector<Token>(expr_len);

    size_t i = 0;

    while(i < expr_len) {
        if (StrLib::isSpace(expr[i])) {
            ++i;
            continue;
        }

        if (StrLib::isDigit(expr[i])) {
            std::string number = StrLib::extractNumber(expr, i);

            infix_tokens.pushBack(Token(TokenType::NUMBER, number));
            continue;
        }

        if (StrLib::isLetter(expr[i])) {
            std::string word = StrLib::extractWord(expr, i);
            std::string lower_word = StrLib::toLower(word);
            TokenType token = getTokenType(lower_word);

            infix_tokens.pushBack(Token(token, lower_word));
            continue;
        }

        if (StrLib::isDot(expr[i])) {
            infix_tokens.pushBack(Token(TokenType::DOT, "."));
            ++i;
            continue;
        }

        if (StrLib::isParen(expr[i])) {
            infix_tokens.pushBack((expr[i] == '(') ? Token(TokenType::LEFT_PAREN, "(") : Token(TokenType::RIGHT_PAREN, ")"));
            ++i;
            continue;
        }

        if (StrLib::isBinaryOperator(expr[i])) {
            TokenType token;
            std::string oper = std::string(1, expr[i]);

            switch(expr[i]) {
                case '+':
                    if (isUnary(infix_tokens.size())) {
                        token = TokenType::UNARY_PLUS;
                    }
                    else {
                        token = TokenType::PLUS;
                    }
                    break;
                case '-':
                    if (isUnary(infix_tokens.size())) {
                        token = TokenType::UNARY_MINUS;
                    }
                    else {
                        token = TokenType::MINUS;
                    }
                    break;
                case '*':
                    token = TokenType::MUL;
                    break;
                case '/':
                    token = TokenType::DIV;
                    break;
            }

            infix_tokens.pushBack(Token(token, oper));
            ++i;
            continue;
        }

        if (StrLib::isComma(expr[i])) {
            infix_tokens.pushBack(Token(TokenType::COMMA, ","));
            ++i;
            continue;
        }
    }

    infix_tokens.shrinkToFit();
}


void Parser::toPostfix() {
    TStack<Token> operators;
    size_t expr_len = infix_tokens.size();

    for (size_t i = 0; i < expr_len; ++i) {
        TokenType type = infix_tokens[i].type;

        if (TokenTraits::isOperand(type)) {
            postfix_tokens.pushBack(infix_tokens[i]);
            continue;
        }

        if (TokenTraits::isOperator(type)) {
            while (!operators.isEmpty() && TokenTraits::getPriority(operators.top().type) >= TokenTraits::getPriority(type)) {
                postfix_tokens.pushBack(operators.top());
                operators.pop();
            }

            operators.push(infix_tokens[i]);
            continue;
        }

        if (TokenTraits::isFunction(type)) {
            operators.push(infix_tokens[i]);
            continue;
        }

        if (type == TokenType::LEFT_PAREN) {
            operators.push(infix_tokens[i]);
            continue;
        }

        if (type == TokenType::COMMA) {
            while (operators.top().type != TokenType::LEFT_PAREN) {
                postfix_tokens.pushBack(operators.top());
                operators.pop();
            }

            continue;
        }

        if (type == TokenType::RIGHT_PAREN) {
            while (operators.top().type != TokenType::LEFT_PAREN) {
                postfix_tokens.pushBack(operators.top());
                operators.pop();
            }

            operators.pop();

            if (!operators.isEmpty() && TokenTraits::isFunction(operators.top().type)) {
                postfix_tokens.pushBack(operators.top());
                operators.pop();
            }

            continue;
        }
    }

    while (!operators.isEmpty()) {
        postfix_tokens.pushBack(operators.top());
        operators.pop();
    }
}


const TVector<Token>& Parser::getPostfix(const std::string& expr) {
    if (!parsed) {
        parse(expr);
        Validator::validation(infix_tokens);
        toPostfix();
        parsed = true;
    }
    
    return postfix_tokens;
}