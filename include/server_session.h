#ifndef SERVER_SESSION_H_GDMSWAAN
#define SERVER_SESSION_H_GDMSWAAN

#include "data_type.h"
#include "server_interface.h"
#include "sockpp/tcp_acceptor.h"
#include "sockpp/tcp_socket.h"

namespace server_session {
class session {

public:

  /**
   * @brief Constructor
   *
   * @param port port to listen on
   */
  session(unsigned port) : acc(port) {}

  /**
   * @brief Start to listen on port.
   *
   */
  void Start();

private:
  unsigned port;
  sockpp::tcp_acceptor acc;

  /**
   * @brief Serve the request from socket to server
   *
   * @param sock src socket
   */
  static void serve(sockpp::tcp_socket sock);

  /**
   * @brief Exec the request and return response
   *
   * @param req request_data from client 
   */
  static data_type::response_data exec(data_type::request_data &);

  static data_type::response_data login(data_type::request_data &req,
                                        std::vector<std::string> &vec);
  static data_type::response_data signup(data_type::request_data &req,
                                         std::vector<std::string> &vec);

  static data_type::response_data add_to_cart(data_type::request_data &req,
                                              std::vector<std::string> &vec);
  static data_type::response_data cart_checkout(data_type::request_data &req,
                                                std::vector<std::string> &vec);
  static data_type::response_data cart_show(data_type::request_data &req,
                                            std::vector<std::string> &vec);
  static data_type::response_data cart_edit(data_type::request_data &req,
                                            std::vector<std::string> &vec);
  static data_type::response_data cart_remove(data_type::request_data &req,
                                              std::vector<std::string> &vec);

  static data_type::response_data orders_show(data_type::request_data &req,
                                              std::vector<std::string> &vec);
  static data_type::response_data orders_pay(data_type::request_data &req,
                                             std::vector<std::string> &vec);
  static data_type::response_data orders_cancel(data_type::request_data &req,
                                                std::vector<std::string> &vec);
  static data_type::response_data list(data_type::request_data &req,
                                       std::vector<std::string> &vec);
  static data_type::response_data search(data_type::request_data &req,
                                         std::vector<std::string> &vec);
  static data_type::response_data passwd(data_type::request_data &req,
                                         std::vector<std::string> &vec);
  static data_type::response_data seller_add(data_type::request_data &req,
                                             std::vector<std::string> &vec);
  static data_type::response_data seller_edit(data_type::request_data &req,
                                              std::vector<std::string> &vec);
  static data_type::response_data seller_list(data_type::request_data &req,
                                              std::vector<std::string> &vec);
  static data_type::response_data seller_remove(data_type::request_data &req,
                                                std::vector<std::string> &vec);
  static data_type::response_data seller_sale(data_type::request_data &req,
                                              std::vector<std::string> &vec);
  static data_type::response_data wallet_show(data_type::request_data &req,
                                              std::vector<std::string> &vec);
  static data_type::response_data wallet_topup(data_type::request_data &req,
                                               std::vector<std::string> &vec);

};
} // namespace server_session

#endif /* end of include guard: SERVER_SESSION_H_GDMSWAAN */
