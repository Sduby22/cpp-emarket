#include <string>
#include <utility>

namespace data_type {


enum class USER_TYPE { CUSTOMER, SELLER };
enum class ITEM_TYPE { BOOK, FOOD, CLOTHING };
enum class REQUEST_TYPE { LOGIN, SIGNUP, PASSWD, LIST, ADD_TO_CART, CART_CHECKOUT, 
  WALLET_SHOW, WALLET_TOPUP, CART_SHOW, ORDERS_SHOW, ORDERS_CANCEL, ORDERS_PAY,
  SELLER_LIST, SELLER_EDIT, SELLER_ADD };

using id_type = unsigned int;

struct base_data {
  virtual void send() = 0;
  virtual void recv() = 0;
  virtual ~base_data();
};

struct request_data: base_data {
  REQUEST_TYPE type;
  id_type user_id;
  id_type target;
  std::string payload1;
  std::string payload2;
};


struct response_data: base_data {
  int success;
  id_type user_id;
  std::string msg;
};


struct user_data: base_data {
  id_type id;
  long long int balance; // Unit: 分
  USER_TYPE type;
  std::string name;
  std::string passwd;
};


struct item_data: base_data {
  id_type id;
  id_type seller;
  size_t stock;
  size_t frozen;
  float discount;
  ITEM_TYPE type;
  long long int price;
  std::string description;
};


struct order_data: base_data {
  id_type id;
  id_type item_id;
  id_type customer;
  id_type seller;
  long long int price;
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
