#include "server_session.h"
#include "data_type.h"
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

using namespace my_user;
using namespace data_type;
using namespace std;

namespace server_session {

void session::Start() {
  db_handler::db_sync();
  while (1) {
    auto sock = acc.accept();

    if (!sock) {
      std::cerr << "error accepting incoming connection: "
                << sock.last_error_str() << std::endl;
    } else {
      std::thread thr(serve, std::move(sock));
      thr.detach();
    }
  }
}

response_data session::exec(request_data &req) {
  response_data resp(1, "success");
  vector<string> vec(base_data::split(req.payload));
  std::cout << req.dump() << std::endl;
  switch (req.type) {
    case REQUEST_TYPE::ADD_TO_CART:
      resp = add_to_cart(req, vec);
      break;
    case REQUEST_TYPE::CART_CHECKOUT:
      resp = cart_checkout(req, vec);
      break;
    case REQUEST_TYPE::CART_SHOW:
      resp = cart_show(req, vec);
      break;
    case REQUEST_TYPE::ORDERS_SHOW:
      resp = orders_show(req, vec);
      break;
    case REQUEST_TYPE::ORDERS_PAY:
      resp = orders_pay(req, vec);
      break;
    case REQUEST_TYPE::ORDERS_CANCEL:
      resp = orders_cancel(req, vec);
      break;
    case REQUEST_TYPE::LIST:
      resp = list(req, vec);
      break;
    case REQUEST_TYPE::SEARCH:
      resp = search(req, vec);
      break;
    case REQUEST_TYPE::LOGIN:
      resp = login(req, vec);
      break;
    case REQUEST_TYPE::SIGNUP:
      resp = signup(req, vec);
      break;
    case REQUEST_TYPE::PASSWD:
      resp = passwd(req, vec);
      break;
    case REQUEST_TYPE::SELLER_ADD:
      resp = seller_add(req, vec);
      break;
    case REQUEST_TYPE::SELLER_EDIT:
      resp = seller_edit(req, vec);
      break;
    case REQUEST_TYPE::SELLER_LIST:
      resp = seller_list(req, vec);
      break;
    case REQUEST_TYPE::WALLET_SHOW:
      resp = wallet_show(req, vec);
      break;
    case REQUEST_TYPE::WALLET_TOPUP:
      resp = wallet_topup(req, vec);
      break;
    default:
      resp.success = false;
      resp.payload = "unknown error";
  }
  return resp;
}

void session::serve(sockpp::tcp_socket sock) {
  request_data req;
  req.recv(sock);

  if (!sock) {
    std::cerr << "error when recving request." << std::endl;
  }

  auto resp = exec(req);
  resp.send(sock);
  if (!sock) {
    std::cerr << "error when sending resp." << std::endl;
  }

  sock.close();
}

data_type::response_data
session::login(request_data &req, vector<string> &vec) {
  auto user = base_user::get(vec[0]);
  if (!user)
    return response_data(0, "user does not exist!");
  else if (!user->checkPass(vec[1]))
    return response_data(0, "wrong password!");
  return response_data(1, user->get_id() , vec[0]);
}

data_type::response_data
session::signup(request_data &req, vector<string> &vec) {
  if (base_user::exist(vec[0])) {
    return response_data(0, "user already exists!");
  }
  auto data = unique_ptr<user_data>
          (new user_data{0, 0, int(req.target), vec[0], vec[1]});
  customer user(std::move(data));
  user.insert();
  return response_data(1, vec[0]);
}

data_type::response_data
session::add_to_cart(request_data &req, vector<string> &vec) {
}

data_type::response_data
session::cart_checkout(request_data &req, vector<string> &vec) {
}

data_type::response_data
session::cart_show(request_data &req, vector<string> &vec) {
}

data_type::response_data
session::orders_show(request_data &req, vector<string> &vec) {
}

data_type::response_data
session::orders_pay(request_data &req, vector<string> &vec) {
}

data_type::response_data
session::orders_cancel(request_data &req, vector<string> &vec) {
}

data_type::response_data
session::list(request_data &req, vector<string> &vec) {
  auto list = base_item::query("");
  string msg;
  for (auto &item: list) {
    msg += item->to_string() + "\n\n";
  }
  return response_data(1, msg);
}

data_type::response_data
session::search(request_data &req, vector<string> &vec) {
  auto list = base_item::query(vec[0]);
  string msg;
  for (auto &item: list) {
    msg += item->to_string() + "\n\n";
  }
  return response_data(1, msg);
}

data_type::response_data
session::passwd(request_data &req, vector<string> &vec) {
  auto user = base_user::get(req.user_id);
  if (!user)
    return response_data(0, "user does not exist!");
  if (user->checkPass(vec[0])) {
    user->changePass(vec[1]);
    return response_data(1, "successfully updated password.");
  } else
    return response_data(0, "incorrect old password.");
}

data_type::response_data
session::seller_add(request_data &req, vector<string> &vec) {
  string name = vec[0];
  string description = vec[1];
  if (name.empty()) name = "item";
  if (description.empty()) description = "No description.";
  int type = req.target;
  long long int price = std::stoll(vec[2]);
  unique_ptr<item_data> data
    (new item_data{0, req.user_id, 1, 0, 1, type, price, description, name});
  base_item item(std::move(data));
  auto id = item.insert();
  return response_data(1, "add succeeded! item id is "+to_string(id));
}

data_type::response_data
session::seller_edit(request_data &req, vector<string> &vec) {
}

data_type::response_data
session::seller_list(request_data &req, vector<string> &vec) {
}

data_type::response_data
session::wallet_show(request_data &req, vector<string> &vec) {
}

data_type::response_data
session::wallet_topup(request_data &req, vector<string> &vec) {
}

} // namespace server_session
