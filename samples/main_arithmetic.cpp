// реализация пользовательского приложения
#include "../include/parser.h"
#include "../include/calculator.h"


#include <iomanip>


int main()
{
  std::string input;

  std::cout << "Enter your expression: ";
  std::getline(std::cin, input);

  Parser parse;

  TVector<Token> postfix = parse.getPostfix(input);

  std::cout << std::fixed << std::setprecision(8) << Calculator::calculate(postfix);
}
