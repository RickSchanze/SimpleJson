//
// Created by 帝皇の惊 on 2022/7/21.
//

#include "simplejson.h"
#include <iostream>
namespace sj {
    bool ListType::json() {
        auto a = base.index();
        if (a == 1) {
            return true;
        }
        return false;
    }

    void ListType::deleteJson() {
        JsonObject* obj = std::get<1>(base);
        delete obj;
        base = nullptr;
    }

    JsonObject::~JsonObject() {
        std::cout << "JsonObject deleted" << std::endl;
    }
} // sj