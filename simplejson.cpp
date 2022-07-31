//
// Created by 帝皇の惊 on 2022/7/21.
//

#include "simplejson.h"
#include <iostream>
namespace sj {
    void ListType::deleteJson() {
        JsonObject* obj = std::get<1>(base);
        delete obj;
        base = nullptr;
    }

    bool ListType::isInt() {
        if (base.index() != 0) return false;
        return std::get<0>(base).index() == 0;
    }

    int ListType::getInt() {
        return std::get<int>(std::get<0>(base));
    }

    bool ListType::isDouble() {
        if (base.index() != 0) return false;
        return std::get<0>(base).index() == 1;
    }

    double ListType::getDouble() {
        return std::get<double>(std::get<0>(base));;
    }

    bool ListType::isBool() {
        if (base.index() != 0) return false;
        return std::get<0>(base).index() == 2;
    }

    bool ListType::getBool() {
        return std::get<bool>(std::get<0>(base));;
    }

    bool ListType::isString() {
        if (base.index() != 0) return false;
        return std::get<0>(base).index() == 3;
    }

    std::string ListType::getString() {
        return std::get<std::string>(std::get<0>(base));;
    }

    bool ListType::isJson() {
        return base.index() == 1;
    }

    JsonObject *ListType::getJson() {
        return nullptr;
    }

    JsonObject::~JsonObject() {
        std::cout << "JsonObject deleted" << std::endl;
    }

    List::List(const std::initializer_list<ListBase> &t) {
        for (auto& temp: t) {
            list.emplace_back(temp);
        }
    }

    List::~List() {
         for (auto& temp: list) {

         }
    }

    ListType &List::operator[](int index) {
        return list[index];
    }

    void List::append(const ListBase &t) {

    }

    void List::earse(int index) {

    }

    void List::clear() {

    }
} // sj