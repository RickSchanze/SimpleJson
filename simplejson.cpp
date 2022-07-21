//
// Created by 帝皇の惊 on 2022/7/21.
//

#include "simplejson.h"
#include <iostream>

namespace sj {

bool ListType::json() {
    auto a = base.index();
    return a == 1;
}

// TODO: Bad idea, do not free memory that was not allocated by yourself.
void ListType::deleteJson() {
    delete std::get<1>(base);
    base = nullptr;
}

JsonObject::~JsonObject() {
    std::cout << "JsonObject deleted." << std::endl;
}

} // namespace sj
