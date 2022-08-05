#include <iostream>
#include <memory>
#include "simplejson.h"
int main() {
    setbuf(stdout,NULL);
    while(true)
    {
        using namespace sj;
        std::shared_ptr<JsonObject> obj(new JsonObject({
                                                 1, 2, 3, true, new JsonObject({1, 2, "nihao", "test"}),
                                                 new JsonObject({
                                                                        {"pi", 3.14}
                                                                })
                                         }));
        std::cout << obj;
        Ptr<List> l(new List({1, 2, 3, new JsonObject{{"pi", 3.1}}, new JsonObject{true, false}}));
        Writer::writeList(std::cout, l.get());
        Ptr<Dict> d(new Dict({{"1", new JsonObject({1, 2, 3})}}));
        Writer::writeDict(std::cout, d.get());
    }
}
