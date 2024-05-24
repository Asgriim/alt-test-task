//
// Created by asgrim on 24.05.24.
//
#include "lib.hpp"
#include <iostream>
int main() {
    Alt::ApiClient apiClient;
//    std::string resp = apiClient.getRequest("https://rdb.altlinux.org/api/version");
    Alt::BranchPackagesArranger arranger;
    Alt::PackageList list;
    Alt::PackageList lis2;
    Alt::BinPackage pack = {.name = "4ti2-libs-debuginfo", .arch = "aarch64"};
    pack.version = "21.2";
    list.push_back(pack);
    pack.version = "21.0";
    lis2.push_back(pack);
    pack.name = "123";
    pack.arch = "3423";
    list.push_back(pack);
    Alt::version_greater(pack,pack);
    auto map = arranger.makeSearchMap(list);
    auto ma2 = arranger.makeSearchMap(lis2);

    auto diff = arranger.findIntersection(map, ma2);

    return 0;
}