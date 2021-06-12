#include "data_type.h"
#include "sockpp/stream_socket.h"
#include <sstream>
#include <vector>

namespace data_type {

template <typename T>
ssize_t base_data::write(sockpp::stream_socket &sock, const T &buf) {
  return sock.write(&buf, sizeof(T));
}

template <>
ssize_t base_data::write(sockpp::stream_socket &sock, const std::string &buf) {
  ssize_t tot = 0;
  unsigned int len = buf.length();
  tot += sock.write(&len, sizeof(unsigned int));
  tot += sock.write(&buf[0], len + 1);
  return tot;
}

template <typename T>
ssize_t base_data::read(sockpp::stream_socket &sock, T &buf) {
  auto len = sock.read(&buf, sizeof(T));
  return len;
}

template <>
ssize_t base_data::read(sockpp::stream_socket &sock, std::string &buf) {
  ssize_t tot = 0;
  unsigned int len;
  tot += read(sock, len);
  buf.resize(len + 1);
  tot += sock.read(&buf[0], len + 1);
  return tot;
}

ssize_t request_data::send(sockpp::stream_socket &sock) const {
  ssize_t tot = 0;
  tot += write(sock, type);
  tot += write(sock, user_id);
  tot += write(sock, target);
  tot += write(sock, payload);
  return tot;
}

ssize_t request_data::recv(sockpp::stream_socket &sock) {
  ssize_t tot = 0;
  tot += read(sock, type);
  tot += read(sock, user_id);
  tot += read(sock, target);
  tot += read(sock, payload);
  return tot;
}

ssize_t response_data::send(sockpp::stream_socket &sock) const {
  ssize_t tot = 0;
  tot += write(sock, success);
  tot += write(sock, user_id);
  tot += write(sock, payload);
  return tot;
}

ssize_t response_data::recv(sockpp::stream_socket &sock) {
  ssize_t tot = 0;
  tot += read(sock, success);
  tot += read(sock, user_id);
  tot += read(sock, payload);
  return tot;
}

std::string request_data::dump() const {
  std::stringstream ss;
  ss << "type: " << int(type) << "\nuser_id: " << user_id
     << "\ntarget: " << target << "\npayload: " << join(split(payload), ' ');
  return ss.str();
}

std::string response_data::dump() const {
  std::stringstream ss;
  ss << "success: " << success << "\nuser_id: " << user_id
     << "\npayload: " << join(split(payload), ' ');
  return ss.str();
}

std::vector<std::string> base_data::split(const std::string &s,
                                          char delimiter) {
  std::vector<std::string> vec;
  auto beg = s.begin();
  auto end = s.end();
  auto mid = std::find(beg, end, delimiter);
  while(mid != end) {
    std::string str(beg, mid);
    vec.push_back(std::move(str));
    beg = mid + 1;
    mid = std::find(beg, end, delimiter);
  }
  std::string str(beg, mid);
  vec.push_back(std::move(str));
  return vec;
}

std::string base_data::join(const std::vector<std::string> &vec,
                            char delimiter) {
  std::string str;
  int i = 0;
  for (auto x : vec) {
    ++i;
    if (i != 1)
      str += delimiter;
    str.append(x);
  }
  return str;
}

} // namespace data_type
