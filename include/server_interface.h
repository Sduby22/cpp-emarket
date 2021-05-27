#ifndef SERVER_INTERFACE_H_48NBXEJM
#define SERVER_INTERFACE_H_48NBXEJM

#include <memory>
#include <string>
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
  data_type::id_type getID() const { return user->id; };
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
  static std::vector<std::unique_ptr<base_item>> query(const std::string &str);
  using ITEM_TYPE = data_type::ITEM_TYPE;
  base_item(std::unique_ptr<data_type::item_data> &&ptr)
    : item(std::move(ptr)) {}
  virtual long long int getPrice() { 
    return static_cast<long long int>(item->price * item->discount); 
  };
  virtual std::string getPriceStr() { 
    return std::to_string(getPrice() / 100); 
  }
  virtual ~base_item() {}
  void update();
  data_type::id_type insert();
private:
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

} // namespace my_user

#endif /* end of include guard: SERVER_INTERFACE_H_48NBXEJM */
