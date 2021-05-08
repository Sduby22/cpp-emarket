#include <string>

template <typename T> struct TypeName;
template<> struct TypeName<int> {static std::string name() {return "int";}};
template<> struct TypeName<double> {static std::string name() {return "double";}};
template<> struct TypeName<float> {static std::string name() {return "float";}};
template<> struct TypeName<std::string> {static std::string name() {return "string";}};
template<> struct TypeName<std::vector<std::string>> {
  static std::string name() {return "list of strings";}
};
/* template<> struct TypeName<int> {static std::string name() {return "int";}};
template<> struct TypeName<int> {static std::string name() {return "int";}};
template<> struct TypeName<int> {static std::string name() {return "int";}};
template<> struct TypeName<int> {static std::string name() {return "int";}};
template<> struct TypeName<int> {static std::string name() {return "int";}}; */
