//
// Created by asgrim on 24.05.24.
//

#include "arranger.hpp"
#include <string_view>
#include <stdexcept>
#include <iostream>
#include <utility>


Alt::BranchPackagesArranger::BranchPackagesArranger(std::string mApiLink) : m_apiLink(std::move(mApiLink)) { }

Alt::BranchPacksDiff
Alt::BranchPackagesArranger::searchUnique(const Alt::ArchSearchMap &map1,
                                          const Alt::ArchSearchMap &map2) {
    Alt::BranchPacksDiff diff;

    for (auto &archIt : map1) {
        for (auto &nameIt : archIt.second) {
            if (!contains(archIt.first, nameIt.first, map2)) {
                diff.comparison[archIt.first.data()].push_back(nameIt.second);
            }
        }
    }
    return std::move(diff);
}

bool
Alt::BranchPackagesArranger::contains(std::string_view arch,
                                      std::string_view name,
                                      const Alt::ArchSearchMap &map) {
    auto archIt = map.find(arch);

    if (archIt != map.end()) {
        auto &nameMap = archIt->second;
        auto nameIt = nameMap.find(name);

        if (nameIt != nameMap.end()) {
            return true;
        }
    }

    return false;
}

Alt::ArchSearchMap Alt::BranchPackagesArranger::makeSearchMap(const Alt::PackageList &list) {
    ArchSearchMap map;
    for (const BinPackage &package : list) {
        map[package.arch][package.name] = package;
    }
    return std::move(map);
}

Alt::BinPackage Alt::BranchPackagesArranger::parseJsonToPackage(const Json::Value &value) {
    BinPackage package;
    package.name = value["name"].asString();
    package.epoch = value["epoch"].asInt64();
    package.version = value["version"].asString();
    package.release = value["release"].asString();
    package.arch = value["arch"].asString();
    package.disttag = value["disttag"].asString();
    package.buildtime = value["buildtime"].asInt64();
    package.source = value["source"].asString();
    return std::move(package);
}

Alt::PackageList Alt::BranchPackagesArranger::parseJsonToPackageList(std::string_view json) {
    PackageList list;
    Json::Value root;
    Json::Reader reader;
    reader.parse(json.data(), root);
    Json::Value packages = root["packages"];
    for (int i = 0; i < packages.size(); ++i) {
        list.push_back(parseJsonToPackage(packages[i]));
    }
    return std::move(list);
}

Alt::BranchPacksDiff
Alt::BranchPackagesArranger::findBranchUniqPacks(std::string_view originBranch, std::string_view comparedBranch) {
    std::string originEndPoint = m_apiLink;
    originEndPoint.append(originBranch);

    std::string comparedEndPoint = m_apiLink;
    comparedEndPoint.append(comparedBranch);

    std::string json = m_apiClient.getRequest(originEndPoint);
    PackageList originList = parseJsonToPackageList(json);

    json = m_apiClient.getRequest(comparedEndPoint);
    PackageList comparedList = parseJsonToPackageList(json);

    Alt::BranchPacksDiff diff = searchUnique(makeSearchMap(originList), makeSearchMap(comparedList));
    diff.branch1 = originBranch;
    diff.branch2 = comparedBranch;

    diff.branchDifference = "only in branch 1";
    return std::move(diff);
}

Alt::TargetDiff
Alt::BranchPackagesArranger::getTargetDiff(std::string_view originBranch, std::string_view comparedBranch) {
    std::string originEndPoint = m_apiLink;
    originEndPoint.append(originBranch);

    std::string comparedEndPoint = m_apiLink;
    comparedEndPoint.append(comparedBranch);

    std::string json = m_apiClient.getRequest(originEndPoint);
    PackageList originList = parseJsonToPackageList(json);

    json = m_apiClient.getRequest(comparedEndPoint);
    PackageList comparedList = parseJsonToPackageList(json);
    TargetDiff targetDiff;

    ArchSearchMap originMap = makeSearchMap(originList);
    ArchSearchMap comparedMap = makeSearchMap(comparedList);

    Alt::BranchPacksDiff onlyBr1 = searchUnique(originMap, comparedMap);
    Alt::BranchPacksDiff onlyBr2 = searchUnique(comparedMap, originMap);
    Alt::BranchPacksDiff verGreater = findIntersection(originMap, comparedMap, version_greater);

    onlyBr1.branch1 = originBranch;
    onlyBr1.branch2 = comparedBranch;
    onlyBr2.branch1 = originBranch;
    onlyBr2.branch2 = comparedBranch;
    verGreater.branch1 = originBranch;
    verGreater.branch2 = comparedBranch;

    onlyBr1.branchDifference = "packages exist only in branch1";
    onlyBr2.branchDifference = "packages exist only in branch2";
    verGreater.branchDifference = "packages version higher in branch 1";

    targetDiff.onlyBr1 = onlyBr1;
    targetDiff.onlyBr2 = onlyBr2;
    targetDiff.verHigherBr1 =verGreater;

    return std::move(targetDiff);



}


bool Alt::version_greater(const Alt::BinPackage &p1, const Alt::BinPackage &p2) {
    return p1.version > p2.version;
}
