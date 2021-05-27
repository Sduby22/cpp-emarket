#ifndef CLIENT_SESSION_H_NMCR7SJP
#define CLIENT_SESSION_H_NMCR7SJP


#include "cli/cli.h"
#include "data_type.h"
#include "cli/clifilesession.h"
#include "sockpp/tcp_connector.h"
#include <memory>
#include <string>

namespace client {

class client_session;

class cli_session {
public:
  friend class client_session;
  cli_session(std::unique_ptr<cli::Menu> menu): cli(std::move(menu)), session(cli) {}
  void Start() { session.Start(); }
  void Exit() { session.Exit(); }
private:
  cli::Cli cli;
  cli::CliFileSession session;
};


class client_session {

public:
  client_session(std::string="localhost", unsigned=12345);
  data_type::response_data feed(const data_type::request_data &req);
  void Start() { session->Start(); }
  bool logged_in() const { return current_user!=0; };

private:
  std::unique_ptr<cli::Menu> gen_main_menu();
  std::unique_ptr<cli::Menu> gen_user_menu();
  std::unique_ptr<cli_session> session;
  data_type::id_type current_user;
  unsigned port;
  std::string host;

  void logout();
  void login(std::string&);
  void signup(std::string&);
  void search(std::string&);
  void list();
  void passwd();
  void add_to_cart(data_type::id_type);
  void cart_checkout();
  void cart_show();
  void wallet_show();
  void wallet_topup(double);
  void orders_show();
  void orders_cancel(data_type::id_type);
  void orders_pay(data_type::id_type);
  void seller_list();
  void seller_edit(data_type::id_type);
  void seller_add();
};

}


#endif /* end of include guard: CLIENT_SESSION_H_NMCR7SJP */
