#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__


#include <iostream>
#include <stdexcept>
#include <cmath>


#include "parser.h"


struct VarValues {
    std::string var_name;
    double var_value;

    VarValues(const std::string& name = "", double value = 0.0);
};


namespace Calculator {
    double fact(double num);
    
    double calculate(TVector<Token>& postfix_tokens);
}


#endif