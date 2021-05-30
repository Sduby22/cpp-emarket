#ifndef SERVER_INTERFACE_H_48NBXEJM
#define SERVER_INTERFACE_H_48NBXEJM

#include <memory>
#include <string>
#include <vector>
#include "data_type.h"
#include "sqlite/sqlite_orm.h"

namespace my_user {

class base_user {
public:
  static bool exist(const std::string& name);
  static std::unique_ptr<base_user> get(data_type::id_type id);
  static std::unique_ptr<base_user> get(const std::string &name);
  using USER_TYPE = data_type::USER_TYPE;
  base_user(std::unique_ptr<data_type::user_data> &&ptr)
    : user(std::move(ptr)) {}
  virtual USER_TYPE getUserType() = 0;
  virtual ~base_user() {}
  void update() const;
  data_type::id_type insert() const;
  bool checkPass(const std::string &pass) const { return pass == user->passwd; }
  void changePass(const std::string &pass) { user->passwd = pass; update(); } 
  data_type::id_type get_id() const { return user->id; };
  std::string get_name() const {return user->name;};
  std::string get_balance() const 
    {
      auto str = std::to_string(user->balance / 100.00);
      str.resize(str.length()-4);
      return str;
    }
  void topup(long long int x) { user->balance += x; update(); };
  bool pay(long long int x) { 
    if (user->balance < x)
      return false;
    user->balance -= x; 
    update();
    return true;
  }
private:
  std::unique_ptr<data_type::user_data> user;
};


class customer: public base_user {
public:
  customer(std::unique_ptr<data_type::user_data> &&ptr)
    : base_user(std::move(ptr)) {}
  virtual USER_TYPE getUserType() { return USER_TYPE::CUSTOMER; }
};


class seller: public base_user {
public:
  seller(std::unique_ptr<data_type::user_data> &&ptr)
    : base_user(std::move(ptr)) {}
  virtual USER_TYPE getUserType() { return USER_TYPE::SELLER; }
};

class base_item {
public:
  static std::unique_ptr<base_item> get(data_type::id_type id);
  static std::vector<std::unique_ptr<base_item>> get_all(data_type::id_type);
  static std::vector<std::unique_ptr<base_item>> 
    get_all(data_type::id_type, data_type::ITEM_TYPE);
  static std::vector<std::unique_ptr<base_item>> query(const std::string &str);
  using ITEM_TYPE = data_type::ITEM_TYPE;
  base_item(std::unique_ptr<data_type::item_data> &&ptr)
    : item(std::move(ptr)) {}
  virtual unsigned long long int getPrice() { 
    return static_cast<long long int>(item->price * item->discount); 
  };
  virtual std::string getPriceStr() { 
    auto pricestr = std::to_string(getPrice());
    switch (pricestr.length()) {
      case 1:
        pricestr.insert(0, "0.0");
        break;
      case 2:
        pricestr.insert(0, "0.");
        break;
      default:
        pricestr.insert(pricestr.end()-2, '.');
    }
    return pricestr;
  }
  virtual ~base_item() {}
  void update();
  data_type::id_type insert();
  std::string to_string();
  std::string get_name() const { return item->name; };
  data_type::id_type get_seller() const { return item->seller; }
  bool edit(std::vector<std::string> &vec);
  void sold(size_t amount) { item->stock -= amount; update();}
  void froze(size_t amount) { item->frozen += amount; update(); }
  void unfroze(size_t amount) { item->frozen -= amount; update(); }
  size_t available_stock() { return item->stock - item->frozen; }
  void sale(double x) { item->discount = x; update(); }
  void remove();
private:
  std::string get_type() const;
  static std::unique_ptr<base_item> get_ptr_from_data(data_type::item_data);
  std::unique_ptr<data_type::item_data> item;
};

class food_item: public base_item {
public:
  food_item(std::unique_ptr<data_type::item_data> &&ptr)
    : base_item(std::move(ptr)) {}
};

class cloth_item: public base_item {
public:
  cloth_item(std::unique_ptr<data_type::item_data> &&ptr)
    : base_item(std::move(ptr)) {}

};

class book_item: public base_item {
public:
  book_item(std::unique_ptr<data_type::item_data> &&ptr)
    : base_item(std::move(ptr)) {}

};

class db_handler {
public:
  static void db_sync();
};

class cart {
  public:
    cart(data_type::id_type user);
    std::string list();
    bool edit(data_type::id_type row_id, size_t quantity);
    data_type::id_type add(data_type::id_type item_id, size_t quantity);
    bool remove(data_type::id_type row_id) { return edit(row_id, 0); }
    bool checkout();
    std::string getPriceStr();
    long long int getPrice();
  private:
    data_type::id_type user;
    std::vector<data_type::item_cart> items;
};

class order {
  public:
    static std::vector<order> get_user_orders(data_type::id_type);
    static std::unique_ptr<order> get(data_type::id_type);
    order(const data_type::order_data &order): data(order) {}
    bool pay();
    void cancel();
    data_type::id_type create();
    std::string to_string() const;
    std::string get_price() const;
  private:
    data_type::order_data data;
};

} // namespace my_user

#endif /* end of include guard: SERVER_INTERFACE_H_48NBXEJM */
