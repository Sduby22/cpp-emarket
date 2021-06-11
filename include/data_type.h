#ifndef DATA_TYPE_H_WXRDIQC3
#define DATA_TYPE_H_WXRDIQC3

#include "sockpp/stream_socket.h"
#include <string>
#include <utility>
#include <vector>

namespace data_type {

enum class USER_TYPE { CUSTOMER, SELLER };
enum class ITEM_TYPE { BOOK, FOOD, CLOTHING };
enum class REQUEST_TYPE {
  LOGIN,
  SIGNUP,
  PASSWD,
  SEARCH,
  LIST,
  ADD_TO_CART,
  CART_CHECKOUT,
  CART_EDIT,
  CART_REMOVE,
  CART_SHOW,
  WALLET_SHOW,
  WALLET_TOPUP,
  ORDERS_SHOW,
  ORDERS_CANCEL,
  ORDERS_PAY,
  SELLER_LIST,
  SELLER_EDIT,
  SELLER_ADD,
  SELLER_REMOVE,
  SELLER_SALE
};

using id_type = unsigned int;

/**
 * @brief base_data supports send() and recv() via socket
 */
struct base_data {
  /**
   * @brief Sends data to socket
   *
   * @param socket socket
   * @return bytes that successfully sent
   */
  virtual ssize_t send(sockpp::stream_socket &socket) const = 0;

  /**
   * @brief Recv data from socket
   *
   * @param socket socket
   * @return bytes that successfully received
   */
  virtual ssize_t recv(sockpp::stream_socket &socket) = 0;

  /**
   * @brief write data to socket
   *
   * @tparam T data type to be written
   * @param sock dest socket
   * @param buf data to be sent
   * @return bytes that successfully sent
   */
  template <typename T>
  static ssize_t write(sockpp::stream_socket &sock, const T &buf);

  /**
   * @brief read data from socket
   *
   * @tparam T data type to be read
   * @param sock src socket
   * @param buf data to be read
   * @return bytes that successfully read
   */
  template <typename T>
  static ssize_t read(sockpp::stream_socket &sock, T &buf);

  /**
   * @brief split string by delimiter into vector<string>
   *
   * @param str src string
   * @param delim delimiter default 27 '\ESC' 
   *
   * @return vector<string> of splitted string
   */
  static std::vector<std::string> split(const std::string &str, char delim= 27);

  /**
   * @brief join a vector to string delimited by delimiter
   *
   * @param vec vector
   * @param delim delimiter
   */
  static std::string join(const std::vector<std::string> &vec, char = 27);

  /**
   * @brief dumps the data to string
   *
   */
  virtual std::string dump() const = 0;

  virtual ~base_data(){};
};

struct request_data : base_data {

  virtual ssize_t send(sockpp::stream_socket &socket) const override;
  virtual ssize_t recv(sockpp::stream_socket &socket) override;
  virtual std::string dump() const override;

  request_data(REQUEST_TYPE type, id_type user_id, id_type target)
      : type(type), user_id(user_id), target(target) {}

  request_data(REQUEST_TYPE type, std::string payload1)
      : type(type), payload(payload1), user_id(0), target(0) {}

  request_data(REQUEST_TYPE type, id_type user_id, id_type target,
               std::string payload1)
      : type(type), user_id(user_id), target(target), payload(payload1) {}

  request_data() {}

  REQUEST_TYPE type;
  id_type user_id;
  id_type target;
  std::string payload;
};

struct response_data : base_data {
  virtual ssize_t send(sockpp::stream_socket &socket) const override;
  virtual ssize_t recv(sockpp::stream_socket &socket) override;
  virtual std::string dump() const override;

  response_data(int success, id_type user_id, std::string msg)
      : success(success), user_id(user_id), payload(msg) {}

  response_data(int success, std::string msg)
      : success(success), payload(msg), user_id(0) {}

  response_data() {}

  int success;
  id_type user_id;
  std::string payload;
};

/**
 * @brief user_data - user data type to store in Database
 * 
 * @see my_user::base_user
 */
struct user_data {
  id_type id;
  long long int balance; // Unit: 分
  int type;
  std::string name;
  std::string passwd;
};

/**
 * @brief item data type to store in Database
 *
 * @see my_user::base_item
 */
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

/**
 * struct order_data - order data type to store in Database
 *
 * @see my_user::order
 */
struct order_data {
  id_type id;
  id_type item_id;
  size_t quantity;
  id_type customer;
  id_type seller;
  unsigned long long int price;
};

/**
 * @brief cart data to store in Database
 *
 * @see my_user::cart
 */
struct item_cart {
  id_type row_id;
  id_type item_id;
  id_type user;
  size_t quantity;
};

} // namespace data_type

#endif /* end of include guard: DATA_TYPE_H_WXRDIQC3 */
