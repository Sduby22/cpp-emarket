#include "client_session.h"
#include "cli/cli.h"
#include <memory>
#include <ostream>
#include <string>

using namespace std;

namespace client {

client_session::client_session() : current_user(0) {
  session = std::make_unique<cli_session>(gen_main_menu());
}

std::unique_ptr<cli::Menu> client_session::gen_main_menu() {
  auto menu = std::make_unique<cli::Menu>("cpp-emarket");

  menu->Insert(
      "login", [&](std::ostream &, string str) { login(str); },
      "Sign In", {"username"});
  menu->Insert(
      "register", [&](std::ostream &, string str) { signup(str); },
      "Sign Up", {"username"});
  return menu;
}

void client_session::signup(std::string &asd) {}

void client_session::login(std::string &asd) {
  std::string passwd;
  cout << "login as " << asd << "..." << endl;
  cout << "password:";
  getline(cin, passwd);
}

} // namespace client
