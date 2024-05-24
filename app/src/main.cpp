//
// Created by asgrim on 24.05.24.
//
#include "alttestlib"
#include <iostream>
#include "jsonMapper.hpp"
int main() {
    Alt::BranchPackagesArranger arranger("https://rdb.altlinux.org/api/export/branch_binary_packages/");
    auto t = arranger.getTargetDiff("p10","p9");

    Json::Value jsonTargetDiff = JsonMapper::targetDiffToJson(t);

    // Convert JSON value to string
    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, jsonTargetDiff);

    // Output the JSON string
    std::cout << jsonString << std::endl;


    return 0;
}