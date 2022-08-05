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
        delete list;
        delete dict;
        list = nullptr;
        dict = nullptr;
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

    void Writer::writeList(std::ostream& os, List *l, int indent, int begin) {
        writeSpace(os, indent, begin - 1);
        os << "[\n";
        for (auto it = l->begin(); it != l->end(); it++) {
            if (it + 1 == l->end()) {
                writeListType(os, *it, indent, begin + 1);
            } else {
                writeListType(os, *it, indent, begin + 1);
                os << ",";
            }
            os << "\n";
        }
        writeSpace(os, indent, begin);
        os << "]";
    }

    void Writer::writeDict(std::ostream& os, Dict *d, int indent, int begin) {
        writeSpace(os, indent, begin - 1);
        os << "{\n";
        size_t len = 0;
        for (auto it = d->begin(); it != d->end(); it++) {
            if (len + 1 == d->size()) {
                writeDictType(os, *it, indent, begin + 1);
            } else {
                writeDictType(os, *it, indent, begin + 1);
                os << ",";
            }
            os << "\n";
        }
        writeSpace(os, indent, begin);
        os << "}";
    }

    void Writer::writeListType(std::ostream& os, const ListType &t, int indent, int begin) {
        writeSpace(os, indent, begin);
        if (t.isInt())
            os << t.getInt();
        if (t.isDouble())
            os << t.getDouble();
        if (t.isBool()) {
            if (t.getBool())
                os << "true";
            else
                os << "false";
        }
        if (t.isString()) os << "\"" << t.getString() << "\"";
        if (t.isJson()) writeJson(os, t.getJson(), indent, begin);
    }

    void Writer::writeSpace(std::ostream& os, int indent, int begin) {
        for (int i = 0; i < begin; i++) {
            for (int j = 0; j < indent; j++) {
                os << " ";
            }
        }
    }

    void Writer::writeJson(std::ostream& os, JsonObject *obj, int indent, int begin) {
        if (obj->isList())
            writeList(os, obj->getList(), indent, begin);
        if (obj->isDict())
            writeDict(os, obj->getDict(), indent, begin);
    }

    void Writer::writeDictType(std::ostream& os, const std::pair<std::string, ListType> &t, int indent, int begin) {
        writeSpace(os, indent, begin);
        os << "\"" << t.first << "\": ";
        if (t.second.isInt())
            os << t.second.getInt();
        if (t.second.isDouble())
            os << t.second.getDouble();
        if (t.second.isBool()) {
            if (t.second.getBool())
                os << "true";
            else
                os << "false";
        }
        if (t.second.isString())
            os << "\"" << t.second.getString() << "\"";
        if (t.second.isJson())
            writeJson(os, t.second.getJson(), indent, begin);
    }

    void Writer::writeJsonPtr(std::ostream &, JsonObject *ptr) {
        printf("%p", ptr);
    }

    std::ostream &operator<<(std::ostream &os, JsonObject *obj) {
        Writer::writeJson(os, obj);
        return os;
    }

    std::ostream &operator<<(std::ostream &os, Ptr<JsonObject> obj) {
        Writer::writeJson(os, obj);
        return os;
    }

    void Writer::writeJson(std::ostream& os, Ptr<JsonObject> obj, int indent, int begin) {
        writeJson(os, obj.get(), indent, begin);
    }


    JsonObject *Reader::fromStringRow(const std::string &jsonStr) {
        return nullptr;
    }

    JsonObject *Reader::fromFileRow(const std::string &fileStr) {
        return nullptr;
    }

    Ptr<JsonObject> Reader::fromStringShared(const std::string &jsonStr) {
        return Ptr<JsonObject>(fromFileRow(jsonStr));
    }

    Ptr<JsonObject> Reader::fromFileShared(const std::string &fileStr) {
        return Ptr<JsonObject>(fromFileRow(fileStr));
    }

    bool Reader::readBool(const std::string &jsonStr, size_t &now, size_t &length) {
        return false;
    }

    int Reader::readInt(const std::string &jsonStr, size_t &now, size_t &length) {
        return 0;
    }

    double Reader::readDouble(const std::string &jsonStr, size_t &now, size_t &length) {
        return 0;
    }

    std::string Reader::readString(const std::string &jsonStr, size_t &now, size_t &length) {
        return std::string();
    }

    JsonObject *Reader::createList(const std::string &jsonStr, size_t &now, size_t &length) {
        return nullptr;
    }

    JsonObject *Reader::createDict(const std::string &jsonStr, size_t &now, size_t &length) {
        return nullptr;
    }
} // sj