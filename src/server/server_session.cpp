#include "server_session.h"
#include "data_type.h"
#include <iostream>
#include <thread>

using namespace data_type;

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
  switch (req.type) {
    case REQUEST_TYPE::ADD_TO_CART:
    case REQUEST_TYPE::CART_CHECKOUT:
    case REQUEST_TYPE::CART_SHOW:
    case REQUEST_TYPE::ORDERS_SHOW:
    case REQUEST_TYPE::ORDERS_PAY:
    case REQUEST_TYPE::ORDERS_CANCEL:
    case REQUEST_TYPE::LIST:
    case REQUEST_TYPE::LOGIN:
    case REQUEST_TYPE::SIGNUP:
    case REQUEST_TYPE::PASSWD:
    case REQUEST_TYPE::SELLER_ADD:
    case REQUEST_TYPE::SELLER_EDIT:
    case REQUEST_TYPE::SELLER_LIST:
    case REQUEST_TYPE::WALLET_SHOW:
    case REQUEST_TYPE::WALLET_TOPUP:
    default:
      resp.success = false;
      resp.msg = "unknown error";
      std::cout << req.dump() << std::endl;
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
} // namespace server_session
