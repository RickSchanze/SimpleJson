## SimpleJson
该仓库是一个使用C++17和其标准库写的一个json解析器，内部使用std::vector, std::unordered_map, std::variant。
## 该库的缺点
+ 使用较难（因为是我练手的项目）
+ dict读入后不按顺序排列
+ 较慢  
+ 后续不打算继续维护

因此，如果您有更好的选择，就不要选择这个库
## 使用方法
```C++
class Reader {
public:
    // 从字符串读取， 返回原始指针
    static JsonObject* fromStringRaw(const std::string& jsonStr);
    // 从文件读取，返回只能指针
    static JsonObject* fromFileRaw(const std::string& fileStr);
    static Ptr<JsonObject> fromStringShared(const std::string& jsonStr);
    static Ptr<JsonObject> fromFileShared(const std::string& fileStr);
};
```

```C++
class Writer {
    public:
    // 这些都是向流输出
        // 默认四格缩进
        friend std::ostream& operator<<(std::ostream& os, JsonObject* obj);
        friend std::ostream& operator<<(std::ostream& os, Ptr<JsonObject> obj);

        // indent控制缩进，begin控制空格，请不要修改传入begin参数
        static void writeJson(std::ostream&, JsonObject* obj, int indent = 4, int begin = 0);
        static void writeJson(std::ostream&, Ptr<JsonObject> obj, int indent = 4, int begin = 0);
        static void writeJsonPtr(std::ostream&, JsonObject* ptr);
        static void writeList(std::ostream&, List* l, int indent = 4, int begin = 0);
        static void writeDict(std::ostream&, Dict* l, int indent = 4, int begin = 0);
};
```
```c++
class ListType {
    // 存储json基本类型，专门写一个是未来防止内存泄露
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
};
```
```c++
class List {
    public:
        List() = default;
        // List l({1, 2, true, "test", new JsonObject({{"21":6}})});
        List(const std::initializer_list<ListBase>& t);
        ~List();

        // 获取元素
        ListType& operator[](size_t index);
        // 尾部添加
        void append(const ListBase& t);
        // 删除
        void remove(size_t index);
        // 清空
        void clear();

        std::vector<ListType>::iterator
        begin() {return list.begin();}

        std::vector<ListType>::iterator
        end() {return list.end();}

        size_t size() {return list.size();}
};
```
```c++
class Dict {
    public:
        // Dict d({{"test", 1}, {"25": new JsonObject({1, true})}});
        Dict(const std::initializer_list<DictType>& t);
        ~Dict();

        std::unordered_map<std::string, ListType>::iterator
        begin() {return dict.begin();}
        std::unordered_map<std::string, ListType>::iterator
        end() {return dict.end();}
        // 删除元素
        void remove(std::string key);
        void clear();
        size_t size() {return dict.size();}
        
        // 获取/添加元素
        ListType& operator[](const std::string& key);
};
```
```C++
class JsonObject {
    public:
        JsonObject(): list(nullptr), dict(nullptr) {}
        JsonObject(List* l): list(l), dict(nullptr) {}
        JsonObject(Dict* d): list(nullptr), dict(d) {}
        JsonObject(List* l, Dict* d): list(l), dict(d) {}
        // JsonObject k({1, 2, true, new JsonObject({...})});
        JsonObject(const std::initializer_list<ListBase>& t);
        // JsonObject j({{"1", new JsonObject(...)}, {"test", 1}});
        JsonObject(const std::initializer_list<std::pair<std::string, ListBase>>& t);
        ~JsonObject();

        bool isList() const;
        List* getList() const;
        bool isDict() const;
        Dict* getDict() const;
        size_t size() const;
};
```
## 使用示例
```c++
// 即main.cpp中的内容
#include <iostream>
#include <memory>
#include <fstream>
#include "simplejson.h"

int main() {
    setbuf(stdout,NULL);
    using namespace sj;
    Ptr<JsonObject> j = Reader::fromFileShared("C:\\Users\\1\\Documents\\Work\\Vscode\\test\\test.json");
    Dict* d = nullptr;
    if (j->isDict())
        d = j->getDict();
    if (d) {
        (*d)["123"] = "ffff";
        (*d)["always"] = false;
        d->remove("123");
        std::cout << d->size() << std::endl;
        std::cout << j << std::endl;
        List* l = (*d)["mistake"].getJson()->getList();
        l->append("you are my only");
        (*l)[3] = -31545.156;
        (*l)[5] = "test";
        l->remove(6);
        std::cout << l->size() << std::endl;
        l->clear();
        std::cout << l->size() << std::endl;
        std::cout << j << std::endl;
        d->clear();
        std::cout << j << std::endl;
    }
}

```
