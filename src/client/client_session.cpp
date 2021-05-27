#include "client_session.h"
#include "cli/cli.h"
#include "sockpp/inet_address.h"
#include <memory>
#include <ostream>
#include <string>

using namespace std;
using data_type::base_data;
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
  menu->Insert("search", [&](std::ostream &, string str) { search(str); },
               "Sign Up", {"item name"});
  menu->Insert("list", [&](std::ostream &) { list(); },
               "List all items");
  return menu;
}

std::unique_ptr<cli::Menu> client_session::gen_user_menu() {
  auto menu = std::make_unique<cli::Menu>("main");
  menu->Insert("logout", [&](std::ostream&){ logout(); });
  menu->Insert("search", [&](std::ostream &, string str) { search(str); },
               "Search for items", {"item name"});
  menu->Insert("list", [&](std::ostream &) { list(); },
               "List all items");
  menu->Insert("cart", [&](std::ostream&, data_type::id_type id)
      { add_to_cart(id); }, "Add item to cart", {"item id"});
  menu->Insert("checkout", [&](std::ostream&)
      { cart_checkout(); }, "Checkout the shopping cart");
  menu->Insert("balance", [&](std::ostream&)
      { wallet_show(); }, "Show wallet balance");
  menu->Insert("topup", [&](std::ostream&, double x)
      { wallet_topup(x); }, "Add item to cart", {"Amount"});
  menu->Insert("orders", [&](std::ostream&)
      { orders_show(); }, "Show all orders");
  menu->Insert("cancel", [&](std::ostream&, data_type::id_type id)
      { orders_cancel(id); }, "Cancel an order", {"order id"});
  menu->Insert("pay", [&](std::ostream&, data_type::id_type id)
      { orders_pay(id); }, "Pay an order", {"order id"});

  auto seller_menu = std::make_unique<cli::Menu>("seller");
  seller_menu->Insert("list", [&](std::ostream&)
      {seller_list();}, "List all selling items");
  seller_menu->Insert("edit", [&](std::ostream&, data_type::id_type id)
      {seller_edit(id);}, "Edit a selling item", {"item id"});
  seller_menu->Insert("add", [&](std::ostream&)
      {seller_add();}, "Add an item to sell");
  menu->Insert(std::move(seller_menu));
  return menu;
}

void client_session::logout() {
  session->Exit();
  session = std::make_unique<cli_session>(gen_main_menu());
  session->Start();
  current_user = 0;
  cout << "logged out." << endl;
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
    request_data req(REQUEST_TYPE::SIGNUP, base_data::join({asd, passwd}));
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
  request_data req(REQUEST_TYPE::LOGIN, base_data::join({asd, passwd}));
  auto resp = feed(req);
  if(resp.success) {
    cout << "logged in as " << resp.payload << " success!" << endl;
    session->Exit();
    current_user = resp.user_id;
    session = std::make_unique<cli_session>(gen_user_menu());
    session->Start();
  }
}

void client_session::search(string &str) const {
  
}

void client_session::list() const {

}

void client_session::passwd() const {

}

void client_session::add_to_cart(data_type::id_type id) const {

}

void client_session::cart_checkout() const {

}

void client_session::cart_show() const {

}

void client_session::wallet_show() const {

}

void client_session::wallet_topup(double x) const {

}

void client_session::orders_show() const {

}

void client_session::orders_cancel(data_type::id_type id) const {

}

void client_session::orders_pay(data_type::id_type id) const {

}

void client_session::seller_list() const {

}

void client_session::seller_edit(data_type::id_type id) const {

}

void client_session::seller_add() const {

}

} // namespace client
