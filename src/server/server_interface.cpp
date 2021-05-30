#include "server_interface.h"
#include "data_type.h"
#include "sqlite/sqlite_orm.h"
#include <memory>
#include <string>

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
        make_column("passwd", &user_data::passwd)),
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
        make_column("name", &item_data::name)),
    make_table(
        "orders",
        make_column("id", &order_data::id, autoincrement(), primary_key()),
        make_column("item_id", &order_data::item_id),
        make_column("quantity", &order_data::quantity),
        make_column("customer", &order_data::customer),
        make_column("seller", &order_data::seller),
        make_column("price", &order_data::price)),
    make_table("item_cart", make_column("item_id", &item_cart::item_id),
               make_column("user", &item_cart::user),
               make_column("quantity", &item_cart::quantity)));

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

  std::unique_ptr<base_item> base_item::get_ptr_from_data(item_data data) {
    auto data_ptr(make_unique<item_data>(data));
    switch(ITEM_TYPE(data_ptr->type)) {
      case ITEM_TYPE::BOOK:
        return make_unique<book_item>(std::move(data_ptr));
      case ITEM_TYPE::FOOD:
        return make_unique<food_item>(std::move(data_ptr));
      case ITEM_TYPE::CLOTHING:
        return make_unique<cloth_item>(std::move(data_ptr));
    }
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

  std::vector<unique_ptr<base_item>> base_item::get_all(id_type seller) {
    vector<unique_ptr<base_item>> item_vec;
    for (auto &item: storage.iterate<item_data>
            (where(c(&item_data::seller) == seller))) {
      item_vec.push_back(get_ptr_from_data(item));
    }
    return item_vec;
  }

  vector<unique_ptr<base_item>> base_item::query(const string &str) {
    vector<unique_ptr<base_item>> vec;
    if (!str.empty()) {
      auto whereNameLike = storage.get_all<item_data>
            (where(like(&item_data::name, "%"+str+"%")));
      for (auto &item: whereNameLike) {
        vec.push_back(get_ptr_from_data(item));
      }
    } else {
      auto whereNameLike = storage.get_all<item_data>();
      for (auto &item: whereNameLike) {
        vec.push_back(get_ptr_from_data(item));
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

  string base_item::to_string() {
    string res;
    res += "id: " + std::to_string(item->id) + "\n";
    res += item->name + "\n";
    res += getPriceStr() + "$";
    res += item->discount == 1 ? "\n" : " (now "
            + std::to_string(int((item->discount - 1) * 100)) + "%)\n";
    res += item->description + "\n";
    auto seller = base_user::get(item->seller);
    res += "seller: " + seller->get_name() + "\n";
    res += "stock: " + std::to_string(item->stock - item->frozen) + "\n";
    res += "type: " + get_type();
    return res;
  }

  string base_item::get_type() const {
    switch(ITEM_TYPE(item->type)) {
      case ITEM_TYPE::BOOK:
        return "book";
      case ITEM_TYPE::CLOTHING:
        return "clothing";
      case ITEM_TYPE::FOOD:
        return "food";
      default:
        return "unknown";
    }
  }

  bool base_item::edit(std::vector<std::string> &vec) {
    try {
      if (!vec[0].empty()) {
        // name
        item->name = vec[0];
      }
      if (!vec[1].empty()) {
        // description
        item->description = vec[1];
      }
      if (!vec[2].empty()) {
        // price
        item->price = std::stoll(vec[2]);
      }
      if (!vec[3].empty()) {
        // type
        item->type = std::stoi(vec[3]);
      }
      if (!vec[4].empty()) {
        // stock
        item->stock = std::stoi(vec[3]);
      }
    } catch (...) {
      return false;
    }
    update();
    return true;
  }
  } // namespace my_user
