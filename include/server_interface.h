#ifndef SERVER_INTERFACE_H_48NBXEJM
#define SERVER_INTERFACE_H_48NBXEJM

#include "data_type.h"
#include "sqlite/sqlite_orm.h"
#include <memory>
#include <string>
#include <vector>

namespace my_user {

/**
 * @brief base user handler class
 */
class base_user {
public:
  using USER_TYPE = data_type::USER_TYPE;

  /**
   * @brief check if a user with [name] already exists.
   *
   */
  static bool exist(const std::string &name);

  /**
   * @brief Factory function to get base_user unique_ptr of a user.
   *
   */
  static std::unique_ptr<base_user> get(data_type::id_type id);
  static std::unique_ptr<base_user> get(const std::string &name);

  /**
   * @brief Use ptr of user_data to create new object
   *
   * @param ptr Used move semantics. Should avoid using it again.
   */
  base_user(std::unique_ptr<data_type::user_data> &&ptr)
      : user(std::move(ptr)) {}

  /**
   * @brief Get user type
   *
   */
  virtual USER_TYPE getUserType() = 0;
  virtual ~base_user() {}


  /**
   * @brief Insert a new user to database
   *
   */
  data_type::id_type insert() const;

  /**
   * @brief Checks if the password is correct
   *
   */
  bool checkPass(const std::string &pass) const { return pass == user->passwd; }

  /**
   * @brief Changes the password.
   *
   * @param pass New password
   */
  void changePass(const std::string &pass) {
    user->passwd = pass;
    update();
  }

  data_type::id_type get_id() const { return user->id; };
  std::string get_name() const { return user->name; };

  /**
   * @brief Gets the balance (unit 1$)
   *
   * For example 10001 -> 100.01
   *
   */
  std::string get_balance() const {
    auto str = std::to_string(user->balance / 100.00);
    str.resize(str.length() - 4);
    return str;
  }

  void topup(long long int amount) {
    user->balance += amount;
    update();
  };

  bool pay(long long int amount) {
    if (user->balance < amount)
      return false;
    user->balance -= amount;
    update();
    return true;
  }

private:
  /**
   * @brief Update user info to database.
   *
   */
  void update() const;

  std::unique_ptr<data_type::user_data> user;
};

/**
 * @brief customer handler class
 */
class customer : public base_user {
public:
  /**
   * @brief Constructor
   *
   * @param ptr !USED MOVE SEMANTICS!
   */
  customer(std::unique_ptr<data_type::user_data> &&ptr)
      : base_user(std::move(ptr)) {}
  virtual USER_TYPE getUserType() { return USER_TYPE::CUSTOMER; }
};

/**
 * @brief seller handler class
 */
class seller : public base_user {
public:
  seller(std::unique_ptr<data_type::user_data> &&ptr)
      : base_user(std::move(ptr)) {}
  virtual USER_TYPE getUserType() { return USER_TYPE::SELLER; }
};

/**
 * @brief base item handler class
 */
class base_item {
public:

  using ITEM_TYPE = data_type::ITEM_TYPE;

  /**
   * @brief Factory function to get ptr of base_item.
   *
   */
  static std::unique_ptr<base_item> get(data_type::id_type id);


  /**
   * @brief Factory function to get vector of all items
   *
   */
  static std::vector<std::unique_ptr<base_item>> get_all(data_type::id_type);
  static std::vector<std::unique_ptr<base_item>> get_all(data_type::id_type,
                                                         data_type::ITEM_TYPE);

  /**
   * @brief Gets vector of ptrs of item that match %str%
   *
   */
  static std::vector<std::unique_ptr<base_item>> query(const std::string &str);

  /**
   * @brief Constructor
   *
   * @param ptr !USED MOVE SEMANTICS!
   */
  base_item(std::unique_ptr<data_type::item_data> &&ptr)
      : item(std::move(ptr)) {}

  /**
   * @brief Gets discounted price. (unit 0.01$)
   *
   */
  virtual unsigned long long int getPrice() const {
    return static_cast<long long int>(item->price * item->discount);
  };

  /**
   * @brief Gets human readable price. 
   *
   * For example: 1001 -> 10.01
   *
   */
  virtual std::string getPriceStr() const {
    auto pricestr = std::to_string(getPrice());
    switch (pricestr.length()) {
    case 1:
      pricestr.insert(0, "0.0");
      break;
    case 2:
      pricestr.insert(0, "0.");
      break;
    default:
      pricestr.insert(pricestr.end() - 2, '.');
    }
    return pricestr;
  }

  virtual ~base_item() {}

  /**
   * @brief Insert a new item to database
   *
   */
  data_type::id_type insert();

  /**
   * @brief Dumps item data to string
   *
   */
  std::string to_string() const;

  std::string get_name() const { return item->name; };
  data_type::id_type get_seller() const { return item->seller; }

  bool edit(std::vector<std::string> &vec);

  void sold(size_t amount) {
    item->stock -= amount;
    update();
  }

  void froze(size_t amount) {
    item->frozen += amount;
    update();
  }

  void unfroze(size_t amount) {
    item->frozen -= amount;
    update();
  }

  size_t available_stock() const { return item->stock - item->frozen; }

  /**
   * @brief Set item discount
   *
   * @param x Discount param
   */
  void sale(double x) {
    item->discount = x;
    update();
  }

  /**
   * @brief remove item from database
   *
   */
  void remove();

private:
  void update();
  std::string get_type() const;
  static std::unique_ptr<base_item> get_ptr_from_data(data_type::item_data);
  std::unique_ptr<data_type::item_data> item;
};

/**
 * @brief food item handler class
 */
class food_item : public base_item {
public:
  food_item(std::unique_ptr<data_type::item_data> &&ptr)
      : base_item(std::move(ptr)) {}
};

/**
 * @brief cloth item handler class
 */
class cloth_item : public base_item {
public:
  cloth_item(std::unique_ptr<data_type::item_data> &&ptr)
      : base_item(std::move(ptr)) {}
};

/**
 * @brief book item handler class
 */
class book_item : public base_item {
public:
  book_item(std::unique_ptr<data_type::item_data> &&ptr)
      : base_item(std::move(ptr)) {}
};

class db_handler {
public:
  static void db_sync();
};

/**
 * @brief cart handler class
 */
class cart {
public:
  /**
   * @brief Constructor cart object of a user
   *
   */
  cart(data_type::id_type user);

  /**
   * @brief Dumps the cart list to string
   *
   */
  std::string list();

  /**
   * @brief Edit a cart row data.
   *
   * @return Return true if success
   */
  bool edit(data_type::id_type row_id, size_t quantity);

  /**
   * @brief Add item to cart. 
   *
   */
  data_type::id_type add(data_type::id_type item_id, size_t quantity);

  /**
   * @brief Remove item from cart
   *
   * @return true if success
   */
  bool remove(data_type::id_type row_id) { return edit(row_id, 0); }

  /**
   * @brief Generate order and clear the cart
   *
   * @return true if success
   */
  bool checkout();

  std::string getPriceStr() const;
  long long int getPrice() const;

private:
  data_type::id_type user;
  std::vector<data_type::item_cart> items;
};

/**
 * @brief order handler class
 */
class order {
public:

  /**
   * @brief Get vector of user orders
   *
   */
  static std::vector<order> get_user_orders(data_type::id_type userid);

  /**
   * @brief Factory function to get an order
   *
   * @param id order id
   */
  static std::unique_ptr<order> get(data_type::id_type id);

  order(const data_type::order_data &order) : data(order) {}

  /**
   * @brief customer pays the order, seller receives the money
   *
   * @return true if success
   */
  bool pay();

  /**
   * @brief User cancels the order
   *
   */
  void cancel();

  /**
   * @brief User creates the order, freeze the stock
   *
   * @return new order id if success, 0 if failed
   */
  data_type::id_type create();

  /**
   * @brief Dumps order data to string
   *
   */
  std::string to_string() const;
  std::string get_price() const;
private:
  data_type::order_data data;
};

} // namespace my_user

#endif /* end of include guard: SERVER_INTERFACE_H_48NBXEJM */
