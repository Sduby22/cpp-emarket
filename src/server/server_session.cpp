#include "server_session.h"
#include "data_type.h"
#include <iostream>
#include <memory>
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

}

data_type::response_data
session::signup(request_data &req, vector<string> &vec) {
  if (base_user::exist(vec[0])) {
    return response_data(0, "User Already Exists!");
  }
  auto data = unique_ptr<user_data>
          (new user_data{0, 0, int(USER_TYPE::CUSTOMER), vec[0], vec[1]});
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
}

data_type::response_data
session::passwd(request_data &req, vector<string> &vec) {
}

data_type::response_data
session::seller_add(request_data &req, vector<string> &vec) {
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
