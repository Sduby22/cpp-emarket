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
      add_to_cart(req, vec);
      break;
    case REQUEST_TYPE::CART_CHECKOUT:
      cart_checkout(req, vec);
      break;
    case REQUEST_TYPE::CART_SHOW:
      cart_show(req, vec);
      break;
    case REQUEST_TYPE::ORDERS_SHOW:
      orders_show(req, vec);
      break;
    case REQUEST_TYPE::ORDERS_PAY:
      orders_pay(req, vec);
      break;
    case REQUEST_TYPE::ORDERS_CANCEL:
      orders_cancel(req, vec);
      break;
    case REQUEST_TYPE::LIST:
      list(req, vec);
      break;
    case REQUEST_TYPE::LOGIN:
      login(req, vec);
      break;
    case REQUEST_TYPE::SIGNUP:
      signup(req, vec);
      break;
    case REQUEST_TYPE::PASSWD:
      passwd(req, vec);
      break;
    case REQUEST_TYPE::SELLER_ADD:
      seller_add(req, vec);
      break;
    case REQUEST_TYPE::SELLER_EDIT:
      seller_edit(req, vec);
      break;
    case REQUEST_TYPE::SELLER_LIST:
      seller_list(req, vec);
      break;
    case REQUEST_TYPE::WALLET_SHOW:
      wallet_show(req, vec);
      break;
    case REQUEST_TYPE::WALLET_TOPUP:
      wallet_topup(req, vec);
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

void
session::login(request_data &req, vector<string> &vec) {

}

void
session::signup(request_data &req, vector<string> &vec) {
  auto data = unique_ptr<user_data>
          (new user_data{0, 0, int(USER_TYPE::CUSTOMER), vec[0], vec[1]});
  customer user(std::move(data));
  // user.insert();
}

void
session::add_to_cart(request_data &req, vector<string> &vec) {
}

void
session::cart_checkout(request_data &req, vector<string> &vec) {
}

void
session::cart_show(request_data &req, vector<string> &vec) {
}

void
session::orders_show(request_data &req, vector<string> &vec) {
}

void
session::orders_pay(request_data &req, vector<string> &vec) {
}

void
session::orders_cancel(request_data &req, vector<string> &vec) {
}

void
session::list(request_data &req, vector<string> &vec) {
}

void
session::passwd(request_data &req, vector<string> &vec) {
}

void
session::seller_add(request_data &req, vector<string> &vec) {
}

void
session::seller_edit(request_data &req, vector<string> &vec) {
}

void
session::seller_list(request_data &req, vector<string> &vec) {
}

void
session::wallet_show(request_data &req, vector<string> &vec) {
}

void
session::wallet_topup(request_data &req, vector<string> &vec) {
}

} // namespace server_session
