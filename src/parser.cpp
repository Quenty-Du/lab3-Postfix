#include "../include/parser.h"


std::string intToString(int val) {
    if (val == 0) return "0";
    std::string result;
    bool negative = (val < 0);
    if (negative) {
         val = -val;
    }
    while (val > 0) {
        result = char('0' + val % 10) + result;
        val /= 10;
    }
    if (negative) {
        result = "-" + result;
    }
    return result;
}


Token::Token(TokenType t, const std::string& val)
    : type(t), value(val) {

}


bool Parser::isSpace(char c) {
    return c == ' ';
}


bool Parser::isDigit(char c) {
    return c >= '0' && c <= '9';
}


bool Parser::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}


bool Parser::isDot(char c) {
    return c == '.';
}


bool Parser::isParen(char c) {
    return c == '(' || c == ')';
}


bool Parser::isBinaryOperator(char c) {
    return c == '+' || c == '-' || c == '/' || c == '*';
}


bool Parser::isComma(char c) {
    return c == ',';
}


bool Parser::isUnary(size_t token_count) {
    return (token_count == 0 || infix_tokens[token_count - 1].type == TokenType::LEFT_PAREN);
}


std::string Parser::extractNumber(const std::string& expr, size_t& pos) {
    std::string result;
    size_t length = expr.length();
    while ((pos < length) && (isDigit(expr[pos]) || isDot(expr[pos]))) {
        result += expr[pos];
        ++pos;
    }

    return result;
}


std::string Parser::extractWord(const std::string& expr, size_t& pos) {
    std::string result;
    size_t length = expr.length();
    while ((pos < length) && (isLetter(expr[pos]) || isDigit(expr[pos]))) {
        result += expr[pos];
        ++pos;
    }

    return result;
}


std::string Parser::toLower(const std::string& input_word) {
    std::string lower_word;
    size_t word_length = input_word.length();
    for (size_t i = 0; i < word_length; ++i) {
        if (input_word[i] >= 'A' && input_word[i] <= 'Z') {
            lower_word += input_word[i] + 32;
        }
        else {
            lower_word += input_word[i];
        }
    }

    return lower_word;
}

TokenType Parser::getTokenType(const std::string& lexeme) {
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
        if (isSpace(expr[i])) {
            ++i;
            continue;
        }

        if (isDigit(expr[i])) {
            std::string number = extractNumber(expr, i);

            infix_tokens.pushBack(Token(TokenType::NUMBER, number));
            continue;
        }

        if (isLetter(expr[i])) {
            std::string word = extractWord(expr, i);
            std::string lower_word = toLower(word);
            TokenType token = getTokenType(lower_word);

            infix_tokens.pushBack(Token(token, word));
            continue;
        }

        if (isDot(expr[i])) {
            infix_tokens.pushBack(Token(TokenType::DOT, "."));
            ++i;
            continue;
        }

        if (isParen(expr[i])) {
            infix_tokens.pushBack((expr[i] == '(') ? Token(TokenType::LEFT_PAREN, "(") : Token(TokenType::RIGHT_PAREN, ")"));
            ++i;
            continue;
        }

        if (isBinaryOperator(expr[i])) {
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

        if (isComma(expr[i])) {
            infix_tokens.pushBack(Token(TokenType::COMMA, ","));
            ++i;
            continue;
        }
    }

    infix_tokens.shrinkToFit();
}


TVector<Token> Parser::toPostfix(const TVector<Token>& infix_tokens) {
    TStack<Token> operators;
    TVector<Token> postfix_form;
    size_t expr_len = infix_tokens.size();

    for (size_t i = 0; i < expr_len; ++i) {
        TokenType type = infix_tokens[i].type;

        if (TokenTraits::isOperand(type)) {
            postfix_form.pushBack(infix_tokens[i]);
            continue;
        }

        if (TokenTraits::isOperator(type)) {
            while (!operators.isEmpty() && TokenTraits::getPriority(operators.top().type) >= TokenTraits::getPriority(type)) {
                postfix_form.pushBack(operators.top());
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
                postfix_form.pushBack(operators.top());
                operators.pop();
            }

            continue;
        }

        if (type == TokenType::RIGHT_PAREN) {
            while (operators.top().type != TokenType::LEFT_PAREN) {
                postfix_form.pushBack(operators.top());
                operators.pop();
            }

            operators.pop();

            if (!operators.isEmpty() && TokenTraits::isFunction(operators.top().type)) {
                postfix_form.pushBack(operators.top());
                operators.pop();
            }

            continue;
        }
    }

    while (!operators.isEmpty()) {
        postfix_form.pushBack(operators.top());
        operators.pop();
    }

    return postfix_form;
}