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

using BaseType = std::variant<int, double, bool, std::string>;
using ListBase = std::variant<BaseType, JsonObject*>;

// 容纳一个 Json 的基本容器
// 新定义是为了防止内存泄漏
class ListType {
public:
    template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, ListBase>>>
    ListType(T t): base(t) { }

    // 判断存的是不是一个 Json
    bool json();

    // 定义复制运算符
    template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, ListBase>>>
    ListType& operator = (const T& t) {
        if (json()) {
            deleteJson();
        }
        base = t;
        return *this;
    }

    // 定义隐式转换，这是基本类型的
    template<typename T, typename = std::enable_if_t<std::is_convertible_v<ListBase, T>>>
    operator T() {
        return std::get<T>(std::get<0>(base));
    }

    // 重载指针类型
    operator JsonObject*() {
        return std::get<1>(base);
    }

    // 不允许复制，因为可能有内存泄漏问题
    ListType(const ListType&) = delete;
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

} // namespace sj

#endif // SIMPLEJSON_SIMPLEJSON_H
