#include "data_type.h"
#include "sockpp/stream_socket.h"
#include <sstream>

namespace data_type {

  template<typename T>
  ssize_t base_data::write(sockpp::stream_socket &sock, const T& buf) {
    return sock.write(&buf, sizeof(T));
  }

  template<>
  ssize_t base_data::write(sockpp::stream_socket &sock, const std::string& buf)
  {
    ssize_t tot = 0;
    unsigned int len = buf.length();
    tot+=sock.write(&len, sizeof(unsigned int));
    tot+=sock.write(&buf[0], len+1);
    return tot;
  }

  template<typename T>
  ssize_t base_data::read(sockpp::stream_socket &sock, T& buf) {
    auto len = sock.read(&buf, sizeof(T));
    return len;
  }

  template<>
  ssize_t base_data::read(sockpp::stream_socket &sock, std::string& buf) {
    ssize_t tot=0;
    unsigned int len;
    tot+=read(sock, len);
    buf.resize(len+1);
    tot+=sock.read(&buf[0], len+1);
    return tot;
  }

  ssize_t request_data::send(sockpp::stream_socket &sock) const {
    ssize_t tot = 0;
    tot += write(sock, type);
    tot += write(sock, user_id);
    tot += write(sock, target);
    tot += write(sock, payload1);
    tot += write(sock, payload2);
    return tot;
  }

  ssize_t request_data::recv(sockpp::stream_socket &sock) {
    ssize_t tot = 0;
    tot += read(sock, type);
    tot += read(sock, user_id);
    tot += read(sock, target);
    tot += read(sock, payload1);
    tot += read(sock, payload2);
    return tot;
  }

  ssize_t response_data::send(sockpp::stream_socket &sock) const {
    ssize_t tot = 0;
    tot += write(sock, success);
    tot += write(sock, user_id);
    tot += write(sock, msg);
    return tot;
  }

  ssize_t response_data::recv(sockpp::stream_socket &sock) {
    ssize_t tot = 0;
    tot += read(sock, success);
    tot += read(sock, user_id);
    tot += read(sock, msg);
    return tot;
  }

  std::string request_data::dump() const {
    std::stringstream ss;
    ss << "type: " << int(type)
       << "\nuser_id: " << user_id
       << "\ntarget: " << target
       << "\npayload1: " << payload1
       << "\npayload2: " << payload2;
    return ss.str();
  }

  std::string response_data::dump() const {
    std::stringstream ss;
    ss << "success: " << success
       << "\nuser_id: " << user_id
       << "\nmsg: " << msg;
    return ss.str();
  }

}
