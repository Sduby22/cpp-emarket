#include <string>
#include "data_type.h"

namespace my_user {

class base_user {

public:
  using USER_TYPE = data_type::USER_TYPE;
  virtual USER_TYPE getUserType() = 0;
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
