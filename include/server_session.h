#ifndef SERVER_SESSION_H_GDMSWAAN
#define SERVER_SESSION_H_GDMSWAAN


#include "sockpp/tcp_acceptor.h"
#include "sockpp/tcp_socket.h"
#include "data_type.h"

namespace server_session {
  class session {
  private:
    unsigned port;
    sockpp::tcp_acceptor acc;
    static void serve(sockpp::tcp_socket sock);
    static data_type::response_data exec(data_type::request_data&);
  public:
    session(unsigned port): acc(port) {}
    void Start();
  };
}


#endif /* end of include guard: SERVER_SESSION_H_GDMSWAAN */
