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

    inline
    bool ListType::isInt() const{
        if (base.index() != 0) return false;
        return std::get<0>(base).index() == 0;
    }

    inline
    int ListType::getInt() const{
        return std::get<int>(std::get<0>(base));
    }

    inline
    bool ListType::isDouble() const{
        if (base.index() != 0) return false;
        return std::get<0>(base).index() == 1;
    }

    inline
    double ListType::getDouble() const{
        return std::get<double>(std::get<0>(base));;
    }

    inline
    bool ListType::isBool() const{
        if (base.index() != 0) return false;
        return std::get<0>(base).index() == 2;
    }

    inline
    bool ListType::getBool() const{
        return std::get<bool>(std::get<0>(base));;
    }

    inline
    bool ListType::isString() const{
        if (base.index() != 0) return false;
        return std::get<0>(base).index() == 3;
    }

    inline
    std::string ListType::getString() const{
        return std::get<std::string>(std::get<0>(base));;
    }

    inline
    bool ListType::isJson() const{
        return base.index() == 1;
    }

    inline
    JsonObject *ListType::getJson() const{
        return std::get<JsonObject*>(base);
    }

    JsonObject::~JsonObject() {
        std::cout << std::endl << "JsonObject deleted" << std::endl;
    }

    List::List(const std::initializer_list<ListBase> &t) {
        for (auto& temp: t) {
            list.emplace_back(temp);
        }
    }

    List::~List() {
         for (auto& temp: list) {
             if (temp.isJson()) temp.deleteJson();
         }
    }

    inline
    ListType &List::operator[](size_t index) {
        return list[index];
    }

    void List::append(const ListBase &t) {
        list.emplace_back(t);
    }

    void List::remove(size_t index) {
        if (list[index].isJson()) list[index].deleteJson();
        list.erase(list.begin() + index);
    }

    void List::clear() {
        for (auto& temp: list) {
            if (temp.isJson()) temp.deleteJson();
        }
        list.clear();
    }

    Dict::Dict(const std::initializer_list<DictType> &t) {
        for (auto& temp: t) {
            dict[temp.first] = temp.second;
        }
    }

    Dict::~Dict() {
        for (auto& temp: dict) {
            if (temp.second.isJson()) temp.second.deleteJson();
        }
    }

    void Dict::remove(std::string key) {
        if (dict[key].isJson()) dict[key].deleteJson();
        dict.erase(key);
    }

    void Dict::clear() {
        for (auto& temp: dict) {
            if (temp.second.isJson()) temp.second.deleteJson();
        }
        dict.clear();
    }

    inline
    ListType& Dict::operator[](const std::string &key) {
        return dict[key];
    }

    std::ostream &Writer::printListType(std::ostream &os, const sj::ListType &t) {
        if (t.isJson())     os << t.getJson();
        if (t.isInt())      os << t.getInt();
        if (t.isDouble())   os << t.getDouble();
        if (t.isBool())     return t.getBool() ? os << "true" : os << "false";
        if (t.isString())   os << "\"" <<t.getString() << "\"";
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const ListType &t) {
        return Writer::printListType(os, t);
    }

    std::ostream& operator<<(std::ostream& os, const std::pair<std::string, ListType>& t) {
        os << "\"" << t.first << "\": ";
        return Writer::printListType(os, t.second);
    }
} // sj