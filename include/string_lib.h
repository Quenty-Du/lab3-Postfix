#ifndef __STRING_LIB_H__
#define __STRING_LIB_H__


#include <string>
#include <cmath>


namespace StrLib {
    bool isSpace(char c) noexcept;
    bool isDigit(char c) noexcept;
    bool isLetter(char c) noexcept;
    bool isDot(char c) noexcept;
    bool isParen(char c) noexcept;
    bool isBinaryOperator(char c) noexcept;
    bool isComma(char c) noexcept;

    bool isNumber(const std::string& input_number);

    std::string toLower(const std::string& input_word);
    std::string extractNumber(const std::string& expr, size_t& pos);
    std::string extractWord(const std::string& expr, size_t& pos);
    std::string intToString(int val);

    double stringToDouble(const std::string& string_double); 
}

#endif