//
// Created by asgrim on 24.05.24.
//
#include "jsonMapper.hpp"

Json::Value JsonMapper::binPackageToJson(const Alt::BinPackage &package) {
    Json::Value jsonPackage;
    jsonPackage["name"] = package.name;
    jsonPackage["epoch"] = Json::Int64(package.epoch);
    jsonPackage["version"] = package.version;
    jsonPackage["release"] = package.release;
    jsonPackage["arch"] = package.arch;
    jsonPackage["disttag"] = package.disttag;
    jsonPackage["buildtime"] = Json::Int64(package.buildtime);
    jsonPackage["source"] = package.source;
    return jsonPackage;
}

Json::Value JsonMapper::packageListToJson(const Alt::PackageList &packageList) {
    Json::Value jsonList(Json::arrayValue);
    for (const auto& package : packageList) {
        jsonList.append(binPackageToJson(package));
    }
    return jsonList;
}

Json::Value JsonMapper::archPackMapToJson(const Alt::ArchPackMap &archPackMap) {
    Json::Value jsonMap(Json::objectValue);
    for (const auto& pair : archPackMap) {
        jsonMap[pair.first] = packageListToJson(pair.second);
    }
    return jsonMap;
}

Json::Value JsonMapper::branchPacksDiffToJson(const Alt::BranchPacksDiff &diff) {
    Json::Value jsonDiff;
    jsonDiff["branch1"] = diff.branch1;
    jsonDiff["branch2"] = diff.branch2;
    jsonDiff["branchDifference"] = diff.branchDifference;
    jsonDiff["comparison"] = archPackMapToJson(diff.comparison);
    return jsonDiff;
}

Json::Value JsonMapper::targetDiffToJson(const Alt::TargetDiff &targetDiff) {
    Json::Value jsonTargetDiff;
    jsonTargetDiff["onlyBr1"] = branchPacksDiffToJson(targetDiff.onlyBr1);
    jsonTargetDiff["onlyBr2"] = branchPacksDiffToJson(targetDiff.onlyBr2);
    jsonTargetDiff["verHigherBr1"] = branchPacksDiffToJson(targetDiff.verHigherBr1);
    return jsonTargetDiff;
}
