#ifndef SERVER_SESSION_H_GDMSWAAN
#define SERVER_SESSION_H_GDMSWAAN


#include "sockpp/tcp_acceptor.h"
#include "sockpp/tcp_socket.h"
#include "data_type.h"
#include "server_interface.h"

namespace server_session {
  class session {
  private:
    unsigned port;
    sockpp::tcp_acceptor acc;
    static void serve(sockpp::tcp_socket sock);
    static data_type::response_data exec(data_type::request_data&);
    static void
    login(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    signup(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    add_to_cart(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    cart_checkout(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    cart_show(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    orders_show(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    orders_pay(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    orders_cancel(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    list(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    passwd(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    seller_add(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    seller_edit(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    seller_list(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    wallet_show(data_type::request_data &req, std::vector<std::string> &vec);
    static void
    wallet_topup(data_type::request_data &req, std::vector<std::string> &vec);
  public:
    session(unsigned port): acc(port) {}
    void Start();
  };
}


#endif /* end of include guard: SERVER_SESSION_H_GDMSWAAN */
