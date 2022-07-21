//
// Created by 帝皇の惊 on 2022/7/21.
//

#ifndef SIMPLEJSON_SIMPLEJSON_H
#define SIMPLEJSON_SIMPLEJSON_H
#include <variant>
#include <string>
#include <type_traits>
namespace sj {
    class JsonObject;
    typedef std::variant<int, double, bool, std::string> BaseType;
    typedef std::variant<BaseType, JsonObject*> ListBase;

    // 容纳一个Json的基本容器
    // 新定义是为了防止内存泄漏
    class ListType {
    public:
        template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, ListBase>>>
        ListType(T t): base(t){ }

        bool json();        // 判断存的是不是一个json

        // 定义复制运算符
        template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, ListBase>>>
        ListType& operator=(const T& t) {
            if (json()) {
                deleteJson();
            }
            base = t;
            return *this;
        }
        // 定义隐式转换，这是基本类型的
        template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, ListBase>>>
        operator T() {
            return std::get<T>(std::get<0>(base));
        }
        // 重载指针类型
        operator JsonObject*() {
            return std::get<1>(base);
        }
        ListType(const ListType&) = delete;     // 不允许复制，因为可能有内存泄漏问题

    private:
        void deleteJson();
        ListBase base;
    };

    class JsonObject {
    public:
        JsonObject() = default;
        ~JsonObject();
    private:
        int a;
    };
} // sj

#endif //SIMPLEJSON_SIMPLEJSON_H
