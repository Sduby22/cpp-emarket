#include "client_session.h"
#include "cli/cli.h"
#include "sockpp/inet_address.h"
#include <memory>
#include <ostream>
#include <string>

using namespace std;
using data_type::request_data;
using data_type::response_data;
using data_type::REQUEST_TYPE;

namespace client {

data_type::response_data 
client_session::feed(const data_type::request_data &req) {
  sockpp::tcp_connector conn;
  response_data resp;
  if(!conn.connect(sockpp::inet_address(host, port))) {
    std::cerr << "failed to connect..." << std::endl;
    resp.success = 0;
  } else {
    req.send(conn);
    resp.recv(conn);
  }
  std::cout << resp.dump() << std::endl;
  conn.close();
  return resp;
}

client_session::client_session(string host, unsigned port) 
  : current_user(0), host(host), port(port) {
  session = std::make_unique<cli_session>(gen_main_menu());
}

std::unique_ptr<cli::Menu> client_session::gen_main_menu() {
  auto menu = std::make_unique<cli::Menu>("cpp-emarket");

  menu->Insert("login", [&](std::ostream &, string str) { login(str); },
               "Sign In", {"username"});
  menu->Insert("register", [&](std::ostream &, string str) { signup(str); },
               "Sign Up", {"username"});
  return menu;
}

void client_session::logout() {
  current_user = 0;
}

void client_session::signup(std::string &asd) {
  std::string passwd, confirm;
  cout << "sign up as " << asd << "..." << endl;
  cout << "set your passwd: ";
  getline(cin, passwd);
  cout << "confirm your passwd: ";
  getline(cin, confirm);
  if (!(passwd==confirm))
    cout << "error: passwords don't match!" << endl;
  else {
    request_data req(REQUEST_TYPE::SIGNUP, asd, passwd);
    auto resp = feed(req);
    if (resp.success)
      current_user = resp.user_id;
  }
}

void client_session::login(std::string &asd) {
  std::string passwd;
  cout << "login as " << asd << "..." << endl;
  cout << "password:";
  getline(cin, passwd);
  request_data req(REQUEST_TYPE::LOGIN, std::move(asd), std::move(passwd));
  auto resp = feed(req);
  if(resp.success)
    current_user = resp.user_id;
}

} // namespace client
