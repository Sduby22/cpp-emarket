#include "server_interface.h"
#include "sqlite/sqlite_orm.h"
#include <memory>

namespace my_user {

  using namespace data_type;
  using namespace sqlite_orm;
  using namespace std;

  auto storage = make_storage(
      "data/db.sqlite",
      make_table(
        "users",
        make_column("id", &user_data::id, autoincrement(), primary_key()),
        make_column("balance", &user_data::balance),
        make_column("type", &user_data::type),
        make_column("name", &user_data::name),
        make_column("passwd", &user_data::passwd)
      ),
      make_table(
        "items",
        make_column("id", &item_data::id, autoincrement(), primary_key()),
        make_column("seller", &item_data::seller),
        make_column("stock", &item_data::stock),
        make_column("frozen", &item_data::frozen),
        make_column("discount", &item_data::discount),
        make_column("type", &item_data::type),
        make_column("price", &item_data::price),
        make_column("description", &item_data::description),
        make_column("name", &item_data::name)
      ),
      make_table(
        "orders",
        make_column("id", &order_data::id, autoincrement(), primary_key()),
        make_column("item_id", &order_data::item_id),
        make_column("customer", &order_data::customer),
        make_column("seller", &order_data::seller),
        make_column("price", &order_data::price)
      )
  );

  std::unique_ptr<base_user> base_user::get(id_type id) {
    auto p = storage.get_pointer<user_data>(id);
    if (!p)
      return nullptr;
    else if (p->type == int(USER_TYPE::CUSTOMER))
      return std::make_unique<customer>(std::move(p));
    else if (p->type == int(USER_TYPE::SELLER))
      return std::make_unique<seller>(std::move(p));
    return nullptr;
  }

  std::unique_ptr<base_user> base_user::get(const std::string &name) {
    auto match = storage.get_all<user_data>(where(c(&user_data::name) == name));
    if (match.empty())
      return nullptr;
    auto p(std::make_unique<user_data>(match[0]));
    if (p->type == int(USER_TYPE::CUSTOMER))
      return std::make_unique<customer>(std::move(p));
    else if (p->type == int(USER_TYPE::SELLER))
      return std::make_unique<seller>(std::move(p));
    return nullptr;
  }

  std::unique_ptr<base_item> base_item::get(id_type id) {
    auto p = storage.get_pointer<item_data>(id);
    if (!p)
      return nullptr;
    switch(ITEM_TYPE(p->type)) {
      case ITEM_TYPE::BOOK:
        return make_unique<book_item>(std::move(p));
      case ITEM_TYPE::FOOD:
        return make_unique<food_item>(std::move(p));
      case ITEM_TYPE::CLOTHING:
        return make_unique<cloth_item>(std::move(p));
    }
    return nullptr;
  }

  static std::vector<std::unique_ptr<base_item>> query(const std::string &str) { 
    auto whereNameLike = storage.get_all<item_data>(where(like(&item_data::name, "%"+str+"%")));
    vector<unique_ptr<base_item>> vec;
    for (auto item: whereNameLike) {
      auto p(std::make_unique<item_data>(item));
      switch(ITEM_TYPE(item.type)) {
        case ITEM_TYPE::CLOTHING:
          vec.emplace_back(make_unique<cloth_item>(std::move(p)));
        case ITEM_TYPE::BOOK:
          vec.emplace_back(make_unique<book_item>(std::move(p)));
        case ITEM_TYPE::FOOD:
          vec.emplace_back(make_unique<food_item>(std::move(p)));
      }
    }
    return vec;
  }

  void base_user::update() const {
    storage.update(*(this->user));
  }

  void base_item::update() {
    storage.update(*(this->item));
  }

  id_type base_item::insert() {
    return storage.insert(*(this->item)); 
  }

  id_type base_user::insert() const {
    return storage.insert(*(this->user));
  }

  void db_handler::db_sync() { 
    storage.sync_schema();
  }

  bool base_user::exist(const std::string& name) {
    auto match = storage.get_all<user_data>(where(c(&user_data::name) == name));
    return !match.empty();
  }

}
