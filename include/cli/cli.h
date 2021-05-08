#include <iterator>
#include <memory>
#include <sstream>
#include <type_traits>
#include <string>
#include <vector>
#include <iostream>
#include "typePrinter.h"
#include "stringParser.h"

namespace cli {

  template <typename F, typename ... Args>
  struct Convert;


  template <typename F, typename First, typename ...Args>
  struct Convert<F, First, Args...> {
    template <typename Iter>
    static void Exec(const F& f, Iter beg, Iter end) {
      const First first = parser::parse_from_string<typename std::decay<First>::type>(*beg);
      auto g = [&](auto ... arg){ f(first, arg...); };
      Convert<decltype(g), Args...>::Exec(g, std::next(beg), end);
    }
  };


  template <typename F>
  struct Convert<F> {
    template <typename Iter>
    static void Exec(const F& f, Iter beg, Iter end) { f(); }
  };


  class BaseCommand {
  public:
    BaseCommand(const std::string& str, const std::string& des="")
      : name(str), description(des) {}
    virtual bool Exec(const std::vector<std::string> &cmd) = 0;
    virtual std::string printHelp() = 0;
  protected:
    const std::string name;
    const std::string description;
  };


  template <typename F, typename ... Args>
  class Command: BaseCommand {
  public:
    Command(const F& f, const std::string& str, const std::string& des="")
      : BaseCommand(str, des) ,f(f) {}

    bool Exec(const std::vector<std::string> &cmd) {
      if (cmd[0] != name) return false;
      else if (cmd.size()-1 != sizeof...(Args)) return false;
      else {
        try {
          Convert<F, Args...>::Exec(f, std::next(cmd.begin()), cmd.end());
          return true;
        } catch(...) {
          std::cout << "command failed: " << "cmd[0]" << std::endl;
          return false;
        }
      }
      return false;
    }

    std::string printHelp() {
      std::stringstream ss;
      ss << name << ' ' << typePrinter<Args...>::getName() << '\n';
      ss << "    "  << description;
      return ss.str();
    }

  private:
    F f;
  };


  class Menu {
  private:
    std::vector<std::unique_ptr<BaseCommand>> cmds;
  };
}
