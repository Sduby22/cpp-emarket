#ifndef CLIENT_SESSION_H_NMCR7SJP
#define CLIENT_SESSION_H_NMCR7SJP

#include "cli/cli.h"
#include "cli/clifilesession.h"
#include "data_type.h"
#include "sockpp/tcp_connector.h"
#include <memory>
#include <string>

namespace client {

class client_session;

/**
 * @brief handles user input, cli menu
 * 
 */
class cli_session {
public:
  cli_session(std::unique_ptr<cli::Menu> menu)
      : cli(std::move(menu)), session(cli) {}

  /**
   * @brief Start the cli session.
   *
   */
  void Start() { session.Start(); }

  /**
   * @brief Exit the cli session
   *
   */
  void Exit() { session.Exit(); }

private:
  cli::Cli cli;
  cli::CliFileSession session;
};

/**
 * @brief handles user input, C-S interaction
 * 
 */
class client_session {

public:
  /**
   * @brief Constructor
   *
   * @param host Server side hostname
   * @param port Server side port
   */
  client_session(std::string host = "localhost", unsigned port = 12345);

  /**
   * @brief feed the request to backend and return response_data
   *
   * IF network version, use socket to communicate with backend server
   * IF client only, just call server_session::exec(req)
   *
   * @see server_session::session::exec
   *
   * @param req request data to exec
   *
   * @return response_data from backend server_session::exec(req)
   */
  data_type::response_data feed(const data_type::request_data &req);

  /**
   * @brief Start the client session
   *
   */
  void Start() { session->Start(); }

  /**
   * @brief Return user login status
   *
   * @return Return true if logged in.
   */
  bool logged_in() const { return current_user != 0; };

private:

/**
 * @brief Generate main menu (login, signup, list etc)
 * 
 * @return std::unique_ptr<cli::Menu> Main menu ptr
 */
  std::unique_ptr<cli::Menu> gen_main_menu();

/**
 * @brief Generate user menu (cart add, orders pay etc)
 * 
 * @return std::unique_ptr<cli::Menu> User menu ptr
 */
  std::unique_ptr<cli::Menu> gen_user_menu();

/**
 * @brief handles user input 
 * 
 */
  std::unique_ptr<cli_session> session;

  /**
   * @brief record login status
   * 
   */
  data_type::id_type current_user;
  data_type::USER_TYPE user_type;

/**
 * @brief server side port
 * 
 */
  unsigned port;

  /**
   * @brief server side hostname
   * 
   */
  std::string host;

  void logout();
  void login(std::string &);
  void signup(std::string &);
  void search(std::string &);
  void list();
  void passwd();
  void add_to_cart(data_type::id_type);
  void cart_checkout();
  void cart_show();
  void cart_remove(data_type::id_type);
  void cart_edit(data_type::id_type);
  void wallet_show();
  void wallet_topup(double);
  void orders_show();
  void orders_cancel(data_type::id_type);
  void orders_pay(data_type::id_type);
  void seller_list();
  void seller_edit(data_type::id_type);
  void seller_add();
  void seller_remove(data_type::id_type);
  void seller_sale();
};

} // namespace client

#endif /* end of include guard: CLIENT_SESSION_H_NMCR7SJP */
