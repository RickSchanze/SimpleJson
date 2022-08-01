//
// Created by 帝皇の惊 on 2022/7/21.
//

#ifndef SIMPLEJSON_SIMPLEJSON_H
#define SIMPLEJSON_SIMPLEJSON_H
#include <variant>
#include <string>
#include <type_traits>
#include <vector>
#include <unordered_map>
#include <initializer_list>
namespace sj {
    class JsonObject;
    typedef std::variant<int, double, bool, std::string> BaseType;
    typedef std::variant<BaseType, JsonObject*> ListBase;

    // 容纳一个Json的基本容器
    // 新定义是为了防止内存泄漏
    class ListType {
    public:
        template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, ListType>>>
        ListType(T t): base(t){ }

        ListType() = default;
        ListType(const ListType&) = default;
        ListType(const ListBase& t): base(t) {}

        // 各种判断
        bool isInt() const;
        int getInt() const;
        bool isDouble() const;
        double getDouble() const;
        bool isBool() const;
        bool getBool() const;
        bool isString() const;
        std::string getString() const;
        bool isJson() const;
        JsonObject* getJson() const;
        // 释放JsonObject*
        void deleteJson();


        // 定义复制运算符
        template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, ListBase>>>
        ListType& operator=(const T& t) {
            if (isJson()) {
                deleteJson();
            }
            base = t;
            return *this;
        }

        // 定义隐式转换，这是基本类型的
        template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, BaseType>>>
        operator T() {
            return std::get<T>(std::get<0>(base));
        }

        // 重载指针类型
        operator JsonObject*() {
            return std::get<1>(base);
        }


    private:
        ListBase base;
    };

    class List {
    public:
        List() = default;
        List(const std::initializer_list<ListBase>& t);
        ~List();

        ListType&operator[](size_t index);
        void append(const ListBase& t);
        void remove(size_t index);
        void clear();

        std::vector<ListType>::iterator
        begin() {return list.begin();}

        std::vector<ListType>::iterator
        end() {return list.end();}

        size_t size() {return list.size();}

    private:
        std::vector<ListType> list;
    };

    typedef std::pair<std::string, ListBase> DictType;

    class Dict {
    public:
        Dict(const std::initializer_list<DictType>& t);
        ~Dict();

        std::unordered_map<std::string, ListType>::iterator
        begin() {return dict.begin();}
        std::unordered_map<std::string, ListType>::iterator
        end() {return dict.end();}
        void remove(std::string key);
        void clear();
        size_t size() {return dict.size();}

        ListType&operator[](const std::string& key);
    private:
        std::unordered_map<std::string, ListType> dict;
    };

    class JsonObject {
    public:
        JsonObject() = default;
        ~JsonObject();
    private:
        int a;
    };

    class Writer {
    public:
        friend std::ostream& operator<<(std::ostream&, const ListType& t);
        friend std::ostream& operator<<(std::ostream&, const std::pair<std::string, ListType>& t);
    private:
        static std::ostream& printListType(std::ostream& os, const ListType& t);
    };

    class Reader {

    };

    std::ostream& operator<<(std::ostream& os, const ListType& t);
    std::ostream& operator<<(std::ostream& os, const std::pair<std::string, ListType>& t);
} // sj

#endif //SIMPLEJSON_SIMPLEJSON_H
