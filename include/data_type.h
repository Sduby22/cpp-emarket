#ifndef DATA_TYPE_H_WXRDIQC3
#define DATA_TYPE_H_WXRDIQC3


#include <string>
#include <utility>
#include <vector>
#include "sockpp/stream_socket.h"

namespace data_type {


enum class USER_TYPE { CUSTOMER, SELLER };
enum class ITEM_TYPE { BOOK, FOOD, CLOTHING };
enum class REQUEST_TYPE { LOGIN, SIGNUP, PASSWD, LIST, ADD_TO_CART, CART_CHECKOUT, 
  WALLET_SHOW, WALLET_TOPUP, CART_SHOW, ORDERS_SHOW, ORDERS_CANCEL, ORDERS_PAY,
  SELLER_LIST, SELLER_EDIT, SELLER_ADD };

using id_type = unsigned int;

struct base_data {
  virtual ssize_t send(sockpp::stream_socket &socket) const = 0;
  virtual ssize_t recv(sockpp::stream_socket &socket) = 0;

  template<typename T>
  static ssize_t write(sockpp::stream_socket &sock, const T& buf);
  
  template<typename T>
  static ssize_t read(sockpp::stream_socket &sock, T& buf);

  static std::vector<std::string> split(const std::string &str, char=27);
  static std::string join(const std::vector<std::string> &vec, char=27);
  
  virtual std::string dump() const = 0;

  virtual ~base_data() {};
};

struct request_data: base_data {
  virtual ssize_t send(sockpp::stream_socket &socket) const;
  virtual ssize_t recv(sockpp::stream_socket &socket);
  virtual std::string dump() const;

  request_data(REQUEST_TYPE type, id_type user_id, id_type target)
    : type(type), user_id(user_id), target(target)
  {}

  request_data(REQUEST_TYPE type, std::string payload1)
    : type(type), payload(payload1), user_id(0), target(0)
  {}

  request_data(REQUEST_TYPE type, id_type user_id, id_type target,
               std::string payload1)
    : type(type), user_id(user_id), target(target), 
      payload(payload1)
  {}

  request_data() {}

  REQUEST_TYPE type;
  id_type user_id;
  id_type target;
  std::string payload;
};


struct response_data: base_data {
  virtual ssize_t send(sockpp::stream_socket &socket) const;
  virtual ssize_t recv(sockpp::stream_socket &socket);
  virtual std::string dump() const;

  response_data(int success, id_type user_id, std::string msg)
    : success(success), user_id(user_id), payload(msg) {}

  response_data(int success, std::string msg)
    : success(success), payload(msg), user_id(0) {}

  response_data() {}

  int success;
  id_type user_id;
  std::string payload;
};


struct user_data {
  id_type id;
  long long int balance; // Unit: 分
  int type;
  std::string name;
  std::string passwd;
};


struct item_data {
  id_type id;
  id_type seller;
  size_t stock;
  size_t frozen;
  float discount;
  int type;
  long long int price;
  std::string description;
  std::string name;
};


struct order_data {
  id_type id;
  id_type item_id;
  size_t quantity;
  id_type customer;
  id_type seller;
  long long int price;
};


struct item_cart {
  id_type item_id;
  id_type user;
  size_t quantity;
};


/* template <typename T>
class data_handler {
public:
  data_handler(const T& data): data(data) {}
  data_handler(T&& data): data(std::move(data)) {}
  T dump() { return std::move(data); }
  void send();
  void receive();
private:
  T data;
};


template<> inline void 
data_handler<response_data>::send() {
  
}

template<> inline void 
data_handler<response_data>::receive() {
  
}

template<> inline void 
data_handler<request_data>::send() {
  
}

template<> inline void 
data_handler<request_data>::receive() {
  
} */

} // namespace data


#endif /* end of include guard: DATA_TYPE_H_WXRDIQC3 */
