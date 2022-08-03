#include <iostream>
#include "simplejson.h"
int main() {
    using namespace sj;
    List l = {1, 2, true, "test", 1.32, new JsonObject, false};
    Dict t = {
            {"test", 1},
            {"nihao", true},
            {"wudi", false},
            {"666", 1.4},
            {"h", new JsonObject}
    };
}
