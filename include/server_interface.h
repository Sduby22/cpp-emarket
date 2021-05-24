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
  virtual USER_TYPE getUserType() = 0;
  virtual ~base_user() {}
private:
  static std::unique_ptr<base_user> get();
  data_type::user_data user;
};


class customer: public base_user {
public:
  virtual USER_TYPE getUserType() { return USER_TYPE::CUSTOMER; }
};


class seller: public base_user {
public:
  virtual USER_TYPE getUserType() { return USER_TYPE::SELLER; }
  
};

} // namespace my_user

#endif /* end of include guard: SERVER_INTERFACE_H_48NBXEJM */
