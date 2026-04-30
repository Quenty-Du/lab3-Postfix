#include "../include/string_lib.h"


bool StrLib::isSpace(char c) noexcept {
    return c == ' ';
}


bool StrLib::isDigit(char c) noexcept {
    return c >= '0' && c <= '9';
}


bool StrLib::isLetter(char c) noexcept {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}


bool StrLib::isDot(char c) noexcept {
    return c == '.';
}


bool StrLib::isParen(char c) noexcept {
    return c == '(' || c == ')';
}


bool StrLib::isBinaryOperator(char c) noexcept {
    return c == '+' || c == '-' || c == '/' || c == '*';
}


bool StrLib::isComma(char c) noexcept {
    return c == ',';
}


bool StrLib::isNumber(const std::string& input_number) {
    size_t dot_count = 0;
    size_t number_len = input_number.length();

    if (input_number[0] != '-' && !isDigit(input_number[0])) {
        return false;
    }

    if (!isDigit(input_number[input_number.length() - 1])) {
        return false;
    }

    for (size_t i = 1; i < number_len; ++i) {
        if (!isDigit(input_number[i]) && !isDot(input_number[i])) {
            return false;
        }
        if (isDot(input_number[i])) {
            ++dot_count;
        }
    }

    if (dot_count > 1) {
        return false;
    }

    return true;
}


std::string StrLib::toLower(const std::string& input_word) {
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


std::string StrLib::extractNumber(const std::string& expr, size_t& pos) {
    size_t begin = pos;
    size_t end = 0;
    size_t length = expr.length();
    while ((pos < length) && (isDigit(expr[pos]) || isDot(expr[pos]))) {
        ++end;
        ++pos;
    }

    return expr.substr(begin, end);
}


std::string StrLib::extractWord(const std::string& expr, size_t& pos) {
    size_t begin = pos;
    size_t end = 0;
    size_t length = expr.length();
    while ((pos < length) && (isLetter(expr[pos]) || isDigit(expr[pos]))) {
        ++end;
        ++pos;
    }

    return expr.substr(begin, end);
}


std::string StrLib::intToString(int val) {
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


double StrLib::stringToDouble(const std::string& string_double) {
    size_t digits_before_dot = 0;
    size_t double_len = string_double.length();
    int str_end = 0;

    int dot = 0;

    bool is_dot = true;
    bool is_negative = false;

    double result = 0.0;
    
    if (string_double[0] == '-') {
        is_negative = true;
        str_end++;
    }

    for (int i = double_len - 1; i >= str_end; --i) {
        if (string_double[static_cast<size_t>(i)] == '.') {
            is_dot = false;
            dot = 1;
            continue;
        }

        if (is_dot) {
            digits_before_dot += 1;
        }

        result += (string_double[i] - '0') * pow(10.0, static_cast<double>(double_len - 1 - dot - i));
    }

    if (!is_dot) {
        result = result / pow(10.0, static_cast<double>(digits_before_dot));
    }

    if (is_negative) {
        result = (-1.0) * result;
    }

    return result;
}


