//
// Created by asgrim on 24.05.24.
//
#include "lib.hpp"
#include <iostream>
int main() {
    Alt::BranchPackagesArranger arranger("https://rdb.altlinux.org/api/export/branch_binary_packages/");
    arranger.findBranchUniqPacks("p10","p10");


    return 0;
}