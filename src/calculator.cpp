#include "../include/calculator.h"
#include "../include/token_traits.h"
#include "../include/string_lib.h"


VarValues::VarValues(const std::string& name, double value) 
: var_name(name),
var_value(value) {
}


double Calculator::fact(double num) {
    if (num < 0) {
        throw std::invalid_argument("Factorial argument must be positive");
    }

    if (std::fabs(num - std::round(num)) > 1e-9) {
        throw std::invalid_argument("Factorial argument must be integer");
    }

    if (num == 0.0 || num == 1.0) {
        return 1.0;
    }

    double result = 1;
    size_t up = static_cast<int>(round(num));
    for (size_t i = 2; i <= up; ++i) {
        result *= static_cast<double>(i);
    }

    return static_cast<double>(result);
}


double Calculator::calculate(TVector<Token>& postfix_tokens) {
    TStack<double> result;
    TVector<VarValues> vars;
    size_t postfix_len = postfix_tokens.size();

    for (size_t i = 0; i < postfix_len; ++i) {
        TokenType type = postfix_tokens[i].type;

        if (type == TokenType::NUMBER) {
            result.push(StrLib::stringToDouble(postfix_tokens[i].value));
            continue;
        }

        if (type == TokenType::CONSTANT) {
            if (StrLib::toLower(postfix_tokens[i].value) == "pi") {
                result.push(3.14159265358979323846);
            }
            else {
                result.push(2.71828182845904523536);
            }
            continue;
        }

        if (type == TokenType::VARIABLE) {
            size_t vars_len = vars.size();

            bool not_find = true;

            for (size_t j = 0; j < vars_len; ++j) {
                if (vars[j].var_name == postfix_tokens[i].value) {
                    result.push(vars[j].var_value);
                    not_find = false;
                }
            }

            if (not_find) {
                std::cout << "PLease, enter " << postfix_tokens[i].value << ":";
                std::string input;
                std::cin >> input;
                std::cout << std::endl;

                if (StrLib::toLower(input) == "pi") {
                    vars.pushBack(VarValues(postfix_tokens[i].value, 3.14159265358979323846));
                }
                else if (StrLib::toLower(input) == "e") {
                    vars.pushBack(VarValues(postfix_tokens[i].value, 2.71828182845904523536));
                }
                else if (StrLib::isNumber(input)) {
                    vars.pushBack(VarValues(postfix_tokens[i].value, StrLib::stringToDouble(input)));
                }
                else {
                    throw std::invalid_argument("Wrong variable value, not a number and not a constant");
                }

                result.push(vars.back().var_value);
            }

            continue;
        }

        if (TokenTraits::isOperator(type)) {
            double last;
            double first;
            switch (type) {
            case TokenType::PLUS:
                last = result.top();
                result.pop();
                first = result.top();
                result.pop();
                result.push(first + last);
                break;
            case TokenType::MINUS:
                last = result.top();
                result.pop();
                first = result.top();
                result.pop();
                result.push(first - last);
                break;
            case TokenType::MUL:
                last = result.top();
                result.pop();
                first = result.top();
                result.pop();
                result.push(first * last);
                break;
            case TokenType::DIV:
                last = result.top();
                result.pop();
                first = result.top();
                result.pop();
                result.push(first / last);
                break;
            case TokenType::UNARY_MINUS:
                first = result.top();
                result.pop();
                result.push(-1.0 * first);
                break;
            }

            continue;
        }

        double first;
        double last = result.top();
        result.pop();
        switch (type) {
        case TokenType::SQRT:
            result.push(sqrt(last));
            break;
        case TokenType::POW:
            first = result.top();
            result.pop();
            result.push(pow(first, last));
            break;
        case TokenType::LOG:
            result.push(log(last));
            break;
        case TokenType::SIN:
            result.push(sin(last));
            break;
        case TokenType::COS:
            result.push(cos(last));
            break;
        case TokenType::TG:
            result.push(tan(last));
            break;
        case TokenType::CTG:
            result.push(1.0 / tan(last));
            break;
        case TokenType::ASIN:
            result.push(asin(last));
            break;
        case TokenType::ACOS:
            result.push(acos(last));
            break;
        case TokenType::ATG:
            result.push(atan(last));
            break;
        case TokenType::ACTG:
            result.push(atan(1.0 / last));
            break;
        case TokenType::SH:
            result.push(sinh(last));
            break;
        case TokenType::CH:
            result.push(cosh(last));
            break;
        case TokenType::TH:
            result.push(tanh(last));
            break;
        case TokenType::CTH:
            result.push(1.0 / tanh(last));
            break;
        case TokenType::ASH:
            result.push(asinh(last));
            break;
        case TokenType::ACH:
            result.push(acosh(last));
            break;
        case TokenType::ATH:
            result.push(atanh(last));
            break;
        case TokenType::ACTH:
            result.push(atanh(1.0 / last));
            break;
        case TokenType::FACT:
            result.push(Calculator::fact(last));
            break;
        }
    }

    return result.top();
}