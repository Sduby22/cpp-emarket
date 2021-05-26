#ifndef SERVER_INTERFACE_H_48NBXEJM
#define SERVER_INTERFACE_H_48NBXEJM

#include <memory>
#include <string>
#include "data_type.h"
#include "sqlite/sqlite_orm.h"

namespace my_user {

class base_user {
public:
  using USER_TYPE = data_type::USER_TYPE;
  base_user(std::unique_ptr<data_type::user_data> &&ptr)
    : user(std::move(ptr)) {}
  virtual USER_TYPE getUserType() = 0;
  virtual ~base_user() {}
  void update();
private:
  static std::unique_ptr<base_user> get(data_type::id_type id);
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

} // namespace my_user

#endif /* end of include guard: SERVER_INTERFACE_H_48NBXEJM */
