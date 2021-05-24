#ifndef CLIENT_SESSION_H_NMCR7SJP
#define CLIENT_SESSION_H_NMCR7SJP


#include "cli/cli.h"
#include "data_type.h"
#include "cli/clifilesession.h"
#include "sockpp/tcp_connector.h"
#include <memory>
#include <string>

namespace client {

class cli_session {
public:
  cli_session(std::unique_ptr<cli::Menu> menu): cli(std::move(menu)), session(cli) {}
  void Start() { session.Start(); }
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
  void logout();
  void login(std::string&);
  void signup(std::string&);

private:
  std::unique_ptr<cli::Menu> gen_main_menu();
  /* using user_ptr = std::unique_ptr<my_user::base_user>;
  user_ptr current_user; */
  std::unique_ptr<cli_session> session;
  data_type::id_type current_user;
  unsigned port;
  std::string host;
};

}


#endif /* end of include guard: CLIENT_SESSION_H_NMCR7SJP */
