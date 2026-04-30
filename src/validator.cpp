#include "../include/validator.h"
#include "../include/token_traits.h"


void Validator::validateOperands(const TVector<Token>& token, size_t pos) {
    if (pos > 0) {
        if (TokenTraits::canStartExpr(token[pos].type) && TokenTraits::canEndExpr(token[pos - 1].type)) {
            throw std::invalid_argument("Missed operator");
        }
    }
}


bool Validator::validateNumbers(const TVector<Token>& token, size_t pos) {
    bool not_skip = token[pos].type == TokenType::NUMBER;

    if (not_skip) {
        if (token[pos].value[token[pos].value.length() - 1] == '.') {
            throw std::invalid_argument("Dot at the ending of the number detected. Number must end with a digit.");
        }
    }

    return not_skip;
}


bool Validator::validateBinaryOperators(const TVector<Token>& token, size_t pos) {
    bool not_skip = token[pos].type >= TokenType::PLUS && token[pos].type <= TokenType::DIV;

    if (not_skip) {
        if (pos == 0 || pos + 1 == token.size() || !TokenTraits::canEndExpr(token[pos - 1].type) || !TokenTraits::canStartExpr(token[pos + 1].type)) { 
            throw std::invalid_argument("Missed operator argument");
        }
    }

    return not_skip;
}


bool Validator::validateBrackets(const TVector<Token>& token, size_t pos, TStack<Validate>& bracket_check, TStack<Validate>& argument_check, bool& expect_func_args) {
    bool first_skip = token[pos].type == TokenType::LEFT_PAREN;
    bool second_skip = token[pos].type == TokenType::RIGHT_PAREN;

    if (first_skip) {
        if (pos + 1 < token.size() && token[pos + 1].type == TokenType::RIGHT_PAREN) {
            throw std::invalid_argument("Empty brackets");
        }
        if (expect_func_args) {
            expect_func_args = false;
            bracket_check.push(Validate::FUNC);
        }
        else {
            bracket_check.push(Validate::PAREN);
        }
    }

    if (second_skip) {
        if (bracket_check.top() == Validate::FUNC) {
            argument_check.pop();
            if (argument_check.top() != Validate::FUNC) {
                throw std::invalid_argument("Not enough function arguments");
            }
            argument_check.pop();
        }

        bracket_check.pop();

        if (bracket_check.isEmpty()) {
            throw std::invalid_argument("Opening parenthesis is missing");
        }

        if (pos + 1 < token.size() && token[pos + 1].type == TokenType::LEFT_PAREN) {
            throw std::invalid_argument("Missed operator or function name");
        }
    }

    return first_skip + second_skip;
}


bool Validator::validateFunctions(const TVector<Token>& token, size_t pos, TStack<Validate>& argument_check, bool& expect_func_args) {
    bool not_skip = TokenTraits::isFunction(token[pos].type);

    if (not_skip) {
        expect_func_args = true;

        if (pos + 1 == token.size() || token[pos + 1].type != TokenType::LEFT_PAREN) {
            throw std::invalid_argument("Missed opening parenthesis of function");
        }

        argument_check.push(Validate::FUNC);

        if (token[pos].type == TokenType::POW) {
            argument_check.push(Validate::ARG);
            argument_check.push(Validate::ARG);
        }
        else {
            argument_check.push(Validate::ARG);
        }
    }

    return not_skip;
}


bool Validator::validateCommas(const TVector<Token>& token, size_t pos, TStack<Validate>& argument_check) {
    bool not_skip = token[pos].type == TokenType::COMMA;

    if (not_skip) {
        if (argument_check.isEmpty()) {
            throw std::invalid_argument("Comma outside the function");
        }
        else if (pos != 0 && pos + 1 < token.size()) {
            if (!TokenTraits::canEndExpr(token[pos - 1].type) || !TokenTraits::canStartExpr(token[pos + 1].type)) {
                throw std::invalid_argument("Invalid operand or missed bracket before unary operator");
            }
        }

        argument_check.pop();

        if (argument_check.top() == Validate::FUNC) {
            throw std::invalid_argument("Not enough function arguments");
        }
    }

    return not_skip;
}


bool Validator::validateUnaryOperators(const TVector<Token>& token, size_t pos) {
    bool not_skip = token[pos].type == TokenType::UNARY_PLUS || token[pos].type == TokenType::UNARY_MINUS;

    if (not_skip) {
        if (pos + 1 == token.size() || !TokenTraits::canStartExpr(token[pos + 1].type)) {
            throw std::invalid_argument("Missed unary operator operand");
        }
    }

    return not_skip;
}


void Validator::validateDots(const TVector<Token>& token, size_t pos) {
    if (token[pos].type == TokenType::DOT) {
        throw std::invalid_argument("DOT in beginning of the word");
    }
}


void Validator::validation(const TVector<Token>& infix_tokens) {
    TStack<Validate> argument_check;
    TStack<Validate> brackets_check;
    bool expect_func_args = false;
    brackets_check.push(Validate::START);

    size_t expr_len = infix_tokens.size();

    for (size_t i = 0; i < expr_len; ++i) {

        validateOperands(infix_tokens, i); // Обязательная проверка

        if (validateNumbers(infix_tokens, i)) {
            continue;
        }
        
        if (validateBinaryOperators(infix_tokens, i)) {
            continue;
        }

        if (validateBrackets(infix_tokens, i, brackets_check, argument_check, expect_func_args)) {
            continue;
        }

        if (validateFunctions(infix_tokens, i, argument_check, expect_func_args)) {
            continue;
        }

        if (validateCommas(infix_tokens, i, argument_check)) {
            continue;
        }
        
        if (validateUnaryOperators(infix_tokens, i)) {
            continue;
        }
        
        validateDots(infix_tokens, i); 
    }

    if (brackets_check.top() != Validate::START) {
        throw std::invalid_argument("Missed closing parenthesis");
    }
}