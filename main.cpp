#include <iostream>
#include "simplejson.h"
int main() {
    using namespace sj;
    ListType t1 = 5, t2 = 5.4, t3 = "you are my boy", t4 = true, t5 = new JsonObject;
    std::cout << t1.isInt() << "\n";
    std::cout << t2.isDouble() << "\n";
    std::cout << t3.isString() << "\n";
    std::cout << t4.isBool() << "\n";
    std::cout << t5.isJson() << "\n";
}
