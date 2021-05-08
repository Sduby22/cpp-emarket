#include "cli/cli.h"
#include <vector>

using namespace std;
using namespace cli;

int c = 0;

void foo(const string& str, double f) {
  cout << str << f << endl;
}

int main() {
  auto f = [](const string& str, double f){foo(str,f);};
  auto b = [](){};
  Command<decltype(b)> cmd(b, "foo", "This is foo()");
  cout << c <<endl;
  vector<string> vec{"foo"};
  auto a = cmd.Exec(vec);
  cout << a << endl;
  cout << c << endl;
  cout << cmd.printHelp();
}
