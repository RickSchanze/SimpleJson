//
// Created by 帝皇の惊 on 2022/7/21.
//

#include "simplejson.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>
namespace sj {
    void ListType::deleteJson() {
        JsonObject* obj = std::get<1>(base);
        delete obj;
        base = nullptr;
    }

    bool ListType::isInt() const{
        if (base.index() != 0) return false;
        return std::get<0>(base).index() == 0;
    }

    int ListType::getInt() const{
        return std::get<int>(std::get<0>(base));
    }

    bool ListType::isDouble() const{
        if (base.index() != 0) return false;
        return std::get<0>(base).index() == 1;
    }

    double ListType::getDouble() const{
        return std::get<double>(std::get<0>(base));;
    }

    bool ListType::isBool() const{
        if (base.index() != 0) return false;
        return std::get<0>(base).index() == 2;
    }

    bool ListType::getBool() const{
        return std::get<bool>(std::get<0>(base));;
    }

    bool ListType::isString() const{
        if (base.index() != 0) return false;
        return std::get<0>(base).index() == 3;
    }

    std::string ListType::getString() const{
        return std::get<std::string>(std::get<0>(base));;
    }

    bool ListType::isJson() const{
        return base.index() == 1;
    }

    JsonObject *ListType::getJson() const{
        return std::get<JsonObject*>(base);
    }

    JsonObject::~JsonObject() {
        delete list;
        delete dict;
        list = nullptr;
        dict = nullptr;
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

    size_t JsonObject::size() const{
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

    ListType& Dict::operator[](const std::string &key){
        return dict[key];
    }

    void Writer::writeList(std::ostream& os, List *l, int indent, int begin) {
        os << "[\n";
        for (auto it = l->begin(); it != l->end(); it++) {
            writeSpace(os , indent, begin + 1);
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
        os << "{\n";
        size_t len = 0;
        for (auto it = d->begin(); it != d->end(); it++) {
            writeSpace(os, indent, begin + 1);
            if (len + 1 == d->size()) {
                writeDictType(os, *it, indent, begin + 1);
            } else {
                writeDictType(os, *it, indent, begin + 1);
                os << ",";
            }
            os << "\n";
            len++;
        }
        writeSpace(os, indent, begin);
        os << "}";
    }

    void Writer::writeListType(std::ostream& os, const ListType &t, int indent, int begin) {
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


    JsonObject *Reader::fromStringRaw(const std::string &jsonStr) {
        size_t now = 0, len = jsonStr.size();
        while (true) {
            if (jsonStr[now] == ' ') now++;
            else if (jsonStr[now] == '\n') now++;
            else if (jsonStr[now] == '\r') now++;
            else break;
        }
        if (jsonStr[now] == '[') return Reader::createList(jsonStr, now, len);
        else if (jsonStr[now] == '{') return Reader::createDict(jsonStr, now, len);
        else throw std::logic_error("a json object must start with '[' or '{'.");
    }

    JsonObject *Reader::fromFileRaw(const std::string &fileStr) {
        std::ifstream fs;
        std::stringstream ss;
        fs.open(fileStr);
        ss << fs.rdbuf();
        JsonObject* obj = fromStringRaw(ss.str());
        fs.close();
        return obj;
    }

    Ptr<JsonObject> Reader::fromStringShared(const std::string &jsonStr) {
        return Ptr<JsonObject>(fromFileRaw(jsonStr));
    }

    Ptr<JsonObject> Reader::fromFileShared(const std::string &fileStr) {
        return Ptr<JsonObject>(fromFileRaw(fileStr));
    }

    bool Reader::readBool(const std::string &jsonStr, size_t &now, size_t &length) {
        if (now + 4 >= length) {
            throw std::out_of_range("bool is not complete!");
        }
        if (jsonStr.substr(now, 4) == "true") {
            now += 4;
            return true;
        } else if (jsonStr.substr(now, 5) == "false") {
            now += 5;
            return false;
        }
        else throw std::logic_error("the json input is not correct, one of its bool values is bad.");
    }

    int Reader::readInt(const std::string &jsonStr, size_t &now, size_t &length) {
        int rtn = 0, neg = 0;
        if (jsonStr[now] == '-') {
            neg = 1;
            now++;
        }
        while ((jsonStr[now] >= '0' && jsonStr[now] <= '9')){
            rtn = rtn * 10 + (jsonStr[now] - '0');
            now++;
        }
        if (neg) rtn = - rtn;
        return rtn;
    }

    double Reader::readDouble(const std::string &jsonStr, size_t &now, size_t &length) {
        double rtn = 0.0;
        int neg = 0, dotcount = 0;
        if (jsonStr[now] == '-') {
            neg = 1;
            now++;
        }
        while ((jsonStr[now] >= '0' && jsonStr[now] <= '9') || jsonStr[now] == '.'){
            if (jsonStr[now] == '.'){
                dotcount++;
                now++;
                continue;
            }
            if (dotcount == 0) {
                rtn = rtn * 10 + (jsonStr[now] - '0');
            } else if (dotcount == 1) {
                rtn = rtn + (jsonStr[now] - '0') * 0.1;
            } else {
                throw std::logic_error("the double number only has one dot.");
            }
            now++;
        }
        return rtn;
    }

    std::string Reader::readString(const std::string &jsonStr, size_t &now, size_t &length) {
        if (jsonStr[now] != '\"') throw std::logic_error("the first character must be \" when parse a string!");
        size_t next = jsonStr.find('\"', now + 1);
        while (true) {
            if (next == jsonStr.npos) throw std::logic_error("the double quotation are not closed");
            // 检查next的下一个是不是"," 或者 ":" 不是的话这个next不做效
            if (jsonStr[next - 1] == '\\') {
                next = jsonStr.find('\"', next + 1);
            } else {
                break;
            }
        }
        std::string rtn = jsonStr.substr(now + 1, next - now - 1);
        now = next + 1;
        return rtn;
    }

    JsonObject *Reader::createList(const std::string &jsonStr, size_t &now, size_t &length) {
        if (jsonStr[now] != '[')
            throw std::logic_error("the first character must be [ when parse a list!");
        List* l = new List({});
        now++;
        JsonObject* rtn = new JsonObject(l);
        try {
            while (now != length) {
                switch (jsonStr[now]) {
                    case 't':
                        if (Reader::readBool(jsonStr, now, length)) {
                            l->append(true);
                        } else {
                            throw std::logic_error("parse bool failed!");
                        }
                        break;
                    case 'f':
                        if (!Reader::readBool(jsonStr, now, length)) {
                            l->append(false);
                        } else {
                            throw std::logic_error("parse bool failed!");
                        }
                        break;
                    case '\"':
                        l->append(Reader::readString(jsonStr, now, length));
                        break;
                    case '[':
                        l->append(Reader::createList(jsonStr, now, length));
                        break;
                    case ']':
                        now++;
                        return rtn;
                    case '{':
                        l->append(Reader::createDict(jsonStr, now, length));
                        break;
                    case '}':
                        throw std::logic_error("wrong nest.");
                    case ' ':
                    case '\n':
                    case ',':
                    case '\r':
                        now++;
                        break;
                    default:
                        auto nextD = jsonStr.find(".", now);
                        auto nextC = jsonStr.find(",", now);
                        if (nextD != jsonStr.npos && nextC != jsonStr.npos) {
                            if (nextD > nextC)
                                l->append(Reader::readInt(jsonStr, now, length));
                            else
                                l->append(Reader::readDouble(jsonStr, now, length));
                        } else if (nextD != jsonStr.npos && nextC == jsonStr.npos)
                            l->append(Reader::readDouble(jsonStr, now, length));
                        else {
                            l->append(Reader::readInt(jsonStr, now, length));
                        }
                }
            }
        } catch (const std::logic_error& e) {
            delete rtn;
            throw e;
        }
        return rtn;
    }

    JsonObject *Reader::createDict(const std::string &jsonStr, size_t &now, size_t &length) {
        if (jsonStr[now] != '{')
            throw std::logic_error("the first character must be { when parse a dict!");
        now++;
        Dict* d = new Dict({});
        JsonObject* rtn = new JsonObject(d);
        int nowpos = 0;
        std::string first;
        try {
            while (now < length) {
                switch (jsonStr[now]) {
                    case 't':
                        if (Reader::readBool(jsonStr, now, length)) {
                            if (nowpos == 0)
                                throw std::logic_error("the key of a dict must be a string!");
                            (*d)[first] = true;
                            nowpos = (nowpos + 1) % 2;
                        } else {
                            throw std::logic_error("parse bool failed!");
                        }
                        break;
                    case 'f':
                        if (!Reader::readBool(jsonStr, now, length)) {
                            if (nowpos == 0)
                                throw std::logic_error("the key of a dict must be a string!");
                            (*d)[first] = false;
                            nowpos = (nowpos + 1) % 2;
                        } else {
                            throw std::logic_error("parse bool failed!");
                        }
                        break;
                    case '\"':
                        if (nowpos == 0) {
                            first = std::move(Reader::readString(jsonStr, now, length));
                            nowpos = (nowpos + 1) % 2;
                        } else {
                            (*d)[first] = std::move(Reader::readString(jsonStr, now, length));
                            nowpos = (nowpos + 1) % 2;
                        }
                        break;
                    case '[':
                        if (nowpos == 0)
                            throw std::logic_error("the key of a dict must be a string!");
                        else {
                            (*d)[first] = createList(jsonStr, now, length);
                            nowpos = (nowpos + 1) % 2;
                        }
                        break;
                    case ']':
                        throw std::logic_error("wrong nest.");
                    case '{':
                        if (nowpos == 0)
                            throw std::logic_error("the key of a dict must be a string!");
                        else {
                            (*d)[first] = createDict(jsonStr, now, length);
                            nowpos = (nowpos + 1) % 2;
                        }
                        break;
                    case '}':
                        now++;
                        return rtn;
                    case ' ':
                    case '\n':
                    case ',':
                    case ':':
                    case '\r':
                        now++;
                        break;
                    default: {
                        auto nextD = jsonStr.find(".", now);
                        auto nextC = jsonStr.find(",", now);
                        if (nowpos == 0) {
                            throw std::logic_error("the key of a dict must be a string!");
                        }
                        if (nextD != jsonStr.npos && nextC != jsonStr.npos) {
                            if (nextD > nextC)
                                (*d)[first] = Reader::readInt(jsonStr, now, length);
                            else
                                (*d)[first] = Reader::readDouble(jsonStr, now, length);
                        } else if (nextD != jsonStr.npos && nextC == jsonStr.npos)
                            (*d)[first] = Reader::readDouble(jsonStr, now, length);
                        else {
                            (*d)[first] = Reader::readInt(jsonStr, now, length);
                        }
                        nowpos = (nowpos + 1) % 2;
                    }
                }
            }
        }
        catch (const std::logic_error& e) {
            delete rtn;
            throw e;
        }
        return rtn;
    }
} // sj