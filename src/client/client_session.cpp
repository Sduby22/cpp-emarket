#include "client_session.h"
#include "cli/cli.h"
#include "data_type.h"
#include "sockpp/inet_address.h"
#include <memory>
#include <ostream>
#include <string>

using namespace std;
using data_type::base_data;
using data_type::id_type;
using data_type::request_data;
using data_type::response_data;
using data_type::REQUEST_TYPE;

namespace client {

typedef long long int int64;
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
  menu->Insert("add", [&](std::ostream&, data_type::id_type id)
      { add_to_cart(id); }, "Add item to cart", {"item id"});
  menu->Insert("passwd", [&](std::ostream&)
      { passwd(); }, "Change the Password");

  auto orders_menu = std::make_unique<cli::Menu>("orders");
  orders_menu->Insert("list", [&](std::ostream&)
      { orders_show(); }, "List all orders");
  orders_menu->Insert("cancel", [&](std::ostream&, data_type::id_type id)
      { orders_cancel(id); }, "Cancel an order", {"order id"});
  orders_menu->Insert("pay", [&](std::ostream&, data_type::id_type id)
      { orders_pay(id); }, "Pay an order", {"order id"});
  menu->Insert(std::move(orders_menu));

  auto cart_menu = std::make_unique<cli::Menu>("cart");
  cart_menu->Insert("checkout", [&](std::ostream&)
      { cart_checkout(); }, "Checkout the shopping cart");
  cart_menu->Insert("list", [&](std::ostream&)
      { cart_show(); }, "List cart items");
  cart_menu->Insert("edit", [&](std::ostream&, data_type::id_type id)
      { cart_edit(id); }, "Edit cart items", {"row_id"});
  cart_menu->Insert("remove", [&](std::ostream&, data_type::id_type id)
      { cart_remove(id); }, "Remove an cart item", {"row_id"});
  menu->Insert(std::move(cart_menu));

  auto wallet_menu = std::make_unique<cli::Menu>("wallet");
  wallet_menu->Insert("balance", [&](std::ostream&)
      { wallet_show(); }, "Show wallet balance");
  wallet_menu->Insert("topup", [&](std::ostream&, double x)
      { wallet_topup(x); }, "Add item to cart", {"Amount"});
  menu->Insert(std::move(wallet_menu));

  auto seller_menu = std::make_unique<cli::Menu>("seller");
  seller_menu->Insert("list", [&](std::ostream&)
      {seller_list();}, "List all selling items");
  seller_menu->Insert("edit", [&](std::ostream&, data_type::id_type id)
      {seller_edit(id);}, "Edit a selling item", {"item id"});
  seller_menu->Insert("add", [&](std::ostream&)
      {seller_add();}, "Add an item to sell");
  seller_menu->Insert("remove", [&](std::ostream&, data_type::id_type id)
      {seller_remove(id);}, "Remove a selling item", {"item id"});
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
  std::string passwd, confirm, typestr;
  int type = 0;
  cout << "sign up as " << asd << "..." << endl;
  cout << "set your passwd: ";
  getline(cin, passwd);
  cout << "confirm your passwd: ";
  getline(cin, confirm);
  cout << "are you a customer(0) or a seller(1)?: ";
  getline(cin, typestr);
  try {
    type = std::stoi(typestr);
    if (type < 0 || type > 1) {
      cout << "error: invalid input" << endl;
      return;
    }
  } catch(...) {
    cout << "error: invalid input" << endl;
    return;
  }
  if (!(passwd==confirm))
    cout << "error: passwords don't match!" << endl;
  else {
    request_data req(REQUEST_TYPE::SIGNUP, 0, type, 
                     base_data::join({asd, passwd}));
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

void client_session::search(string &str) {
  request_data req(REQUEST_TYPE::SEARCH, str);
  auto resp = feed(req);
}

void client_session::list() {
  request_data req(REQUEST_TYPE::LIST, current_user, 0);
  auto resp = feed(req);
}

void client_session::passwd() {
  std::string _old, _new;
  cout << "old password: ";
  cin >> _old;
  cout << "new password: ";
  cin >> _new;
  request_data req(REQUEST_TYPE::PASSWD, current_user, 0, 
      base_data::join({_old, _new}));
  auto resp = feed(req);
}

void client_session::add_to_cart(data_type::id_type id) {
  string quantity;
  cout << "how many to add to cart: ";
  getline(cin, quantity);
  request_data req(REQUEST_TYPE::ADD_TO_CART, current_user, id, quantity);
  auto resp = feed(req);
}

void client_session::cart_checkout() {

}

void client_session::cart_show() {
  request_data req(REQUEST_TYPE::CART_SHOW, current_user, 0);
  auto resp = feed(req);
}

void client_session::cart_edit(id_type id) {
  string quantity;
  cout << "new quantity: ";
  getline(cin, quantity);
  auto req = request_data(REQUEST_TYPE::CART_EDIT, current_user, id, quantity);
  auto resp = feed(req);
}

void client_session::cart_remove(id_type id) {
  auto req = request_data(REQUEST_TYPE::CART_REMOVE, current_user, id);
  auto resp = feed(req);
}

void client_session::wallet_show() {
  request_data req(REQUEST_TYPE::WALLET_SHOW, current_user, 0);
  auto resp = feed(req);
}

void client_session::wallet_topup(double x) {
  int64 amount = x*100;
  request_data req(REQUEST_TYPE::WALLET_TOPUP, current_user, 
                    0, to_string(amount));
  auto resp = feed(req);
}

void client_session::orders_show() {

}

void client_session::orders_cancel(data_type::id_type id) {

}

void client_session::orders_pay(data_type::id_type id) {

}

void client_session::seller_list() {
  request_data req(REQUEST_TYPE::SELLER_LIST, current_user, 0);
  auto resp = feed(req);
}

void client_session::seller_edit(data_type::id_type id) {
  string name,description,price_str,type_str,stock;
  double price;
  int type;
  cout << "input new name (default unchange): ";
  getline(cin, name);
  cout << "input new description (default unchange): ";
  getline(cin, description);
  cout << "input new price (default unchange): ";
  getline(cin, price_str);
  cout << "input new type book(0), food(1), clothing(2) (default unchange): ";
  getline(cin, type_str);
  cout << "input stock: ";
  getline(cin, stock);
  try {
    if (!price_str.empty())
      price = stod(price_str);
  } catch(...) {
    cout << "error: invalid input" << endl;
    return;
  }
  request_data req(REQUEST_TYPE::SELLER_EDIT, current_user, id
    , base_data::join({name, description, 
      price_str.empty() ? "" : to_string(int64(price*100)), 
                        to_string(type), stock}));
  auto resp = feed(req);
}

void client_session::seller_add() {
  string name, description, price_str, type_str;
  double price;
  int type;
  cout << "input item name: ";
  getline(cin, name);
  cout << "input item description: ";
  getline(cin, description);
  cout << "input item price: ";
  getline(cin, price_str);
  cout << "input item type: book(0), food(1), clothing(2): ";
  getline(cin, type_str);
  try {
    type = stoi(type_str);
    price = stod(price_str);
  } catch(...) {
    cout << "error: invalid input" << endl;
    return;
  }
  request_data req(REQUEST_TYPE::SELLER_ADD, current_user, type
    , base_data::join({name, description, to_string(int64(price*100))}));
  auto resp = feed(req);
}

void client_session::seller_remove(id_type id) {
  auto resp = feed(request_data(REQUEST_TYPE::SELLER_REMOVE, current_user, id));
}

} // namespace client
