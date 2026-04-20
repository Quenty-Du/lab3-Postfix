#include <arithmetic.h>

ArithmeticExpression::Token::Token(TokenType t, const std::string& val)
    : type(t), value(val) {

}


void ArithmeticExpression::parse(const std::string& expr) {

}


static bool ArithmeticExpression::isSpace(char c) {
    return c == ' ';
}


static bool ArithmeticExpression::isDigit(char c) {
    return c >= '0' && c <= '9';
}


static bool ArithmeticExpression::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}


static bool ArithmeticExpression::isDot(char c) {
    return c == '.';
}


static bool ArithmeticExpression::isParen(char c) {
    return c == '(' || c == ')';
}


static bool ArithmeticExpression::isBinaryOperator(char c) {
    return c == '+' || c == '-' || c == '/' || c == '*';
}


static bool ArithmeticExpression::isComma(char c) {
    return c == ',';
}


static bool ArithmeticExpression::isUnary(char c, size_t token_count) {
    return (token_count == 0 || infix_tokens[token_count - 1].type == TokenType)
}


static std::string ArithmeticExpression::extractNumber(const std::string& expr, size_t& pos) {
    std::string result;
    size_t length = expr.length();
    while ((pos < length) && (isDigit(expr[pos]) || isDot(expr[pos]))) {
        result += expr[pos];
        ++pos;
    }

    return result;
}


static std::string ArithmeticExpression::extractWord(const std::string& expr, size_t& pos) {
    std::string result;
    size_t length = expr.length();
    while ((pos < length) && (isLetter(expr[pos]) || isDigit(expr[pos]))) {
        result += expr[pos];
        ++pos;
    }

    return result;
}


static std::string ArithmeticExpression::toLower(const std::string& input_word) {
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

static TokenType ArithmeticExpression::getTokenType(const std::string& lexeme) {
    static const char*[] function_names = {
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


void ArithmeticExpression::parse(const std::string& expr) {
    infix_tokens = TVector<Token>();
    const expression_length = expr.length();
    size_t i = 0;
    size_t token_count = 0;

    while(i < expression_length) {
        if (isSpace(expr[i])) {
            ++i;
        }

        if (isDigit(expr[i])) {
            std::string number = extractNumber(expr, i);

            infix_tokens.resize(infix_tokens.size() + 1);
            infix_tokens[token_count++] = Token(TokenType::NUMBER, number);
        }

        if (isLetter(expr[i])) {
            std::string word = extractWord(expr, i);
            std::string lower_word = toLower(word);
            TokenType token = getTokenType(lower_word);

            infix_tokens.resize(infix_tokens.size() + 1);
            infix_tokens[token_count++] = Token(token, word)
        }

        if (isDot(expr[i])) {
            infix_tokens.resize(infix_tokens.size() + 1);
            infix_tokens[token_count++] = Token(TokenType::DOT, ".");
            ++i;
        }

        if (isParen(expr[i])) {
            infix_tokens.resize(infix_tokens.size() + 1);
            infix_tokens[token_count++] = (expr[i] == '(') ? Token(TokenType::LEFT_PAREN, "(") : Token(TokenType::RIGHT_PAREN, ")");
            ++i;
        }

        if (isBinaryOperator(expr[i])) {
            infix_tokens.resize(infix_tokens.size() + 1);
            TokenType token;
            std::string oper = std::string(1, expr[i]);

            switch(expr[i]) {
                case '+':
                    if (i == 0 || infix_tokens[token_count - 1].type == TokenType::LEFT_PAREN || infix_tokens[token_count - 1].type == TokenType::COMMA) {
                        token = TokenType::UNARY_PLUS;
                    }
                    else {
                        token = TokenType::PLUS;
                    }
                    break;
                case '-':
                    if (i == 0 || infix_tokens[token_count - 1].type == TokenType::LEFT_PAREN || infix_tokens[token_count - 1].type == TokenType::COMMA) {
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

            infix_tokens[token_count++] = Token(token, oper);
            ++i;
        }
    }
}