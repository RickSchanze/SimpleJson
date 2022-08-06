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
