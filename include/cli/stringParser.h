#include <string>
#include <utility>
#include <typeinfo>
#include <iostream>

namespace parser {

  template <typename T>
  inline T parse_from_string(const std::string& str) {
    std::cout << typeid(T).name() << std::endl;
    return T();
  }

  template <>
  inline long long parse_from_string(const std::string& str) {
    return std::stoll(str);
  }

  template <>
  inline int parse_from_string(const std::string& str) {
    return std::stoi(str);
  }

  template <>
  inline std::string parse_from_string(const std::string &str) {
    return std::string(std::move(str));
  }

  template <>
  inline float parse_from_string(const std::string &str) {
    return std::stof(str);
  }

  template <>
  inline double parse_from_string(const std::string &str) {
    return std::stod(str);
  }

}
