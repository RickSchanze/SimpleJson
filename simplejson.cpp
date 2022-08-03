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

    JsonObject::JsonObject(const std::initializer_list<ListBase> &t) {
        list = new List(t);
        dict = nullptr;
    }

    JsonObject::JsonObject(const std::initializer_list<std::pair<std::string, ListBase>> &t) {
        dict = new Dict(t);
        list = nullptr;
    }

    bool JsonObject::isList() const{
        return list != nullptr;
    }

    List *JsonObject::getList() const{
        return list;
    }

    bool JsonObject::isDict() const{
        return dict != nullptr;
    }

    Dict *JsonObject::getDict() const{
        return dict;
    }

    ListType &JsonObject::operator[](size_t index) {
        if (isDict()) throw std::logic_error("this JsonObject contains a Dict, not List");
        return (*list)[index];
    }

    ListType &JsonObject::operator[](const std::string &key) {
        if (isList()) throw std::logic_error("this JsonObject contains a List, not Dict");
        return (*dict)[key];
    }

    size_t JsonObject::size() {
        return this->isList() ? this->getList()->size()
                              : this->getDict()->size();
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

    inline
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
    ListType& Dict::operator[](const std::string &key){
        return dict[key];
    }

} // sj