#include <string>
#include <sstream>
#include <typeinfo>
#include <type_traits>
#include "typeNames.h"

template <typename ... Args>
struct typePrinter;

template <typename First, typename ... Args>
struct typePrinter<First, Args...> {
  static std::string getName() {
    std::stringstream ss;
    ss << '<' << TypeName<typename std::decay<First>::type>::name() << "> " 
       << typePrinter<Args...>::getName();
    return ss.str();
  }
};

template <typename Last>
struct typePrinter<Last> {
  static std::string getName() {
    std::stringstream ss;
    ss << '<' << TypeName<typename std::decay<Last>::type>::name() << '>';
    return ss.str();
  }
};

template <>
struct typePrinter<> {
  static std::string getName() {
    return "";
  }
};

