//
// Created by asgrim on 24.05.24.
//

#ifndef ALT_TEST_JSONMAPPER_HPP
#define ALT_TEST_JSONMAPPER_HPP

#include <jsoncpp/json/json.h>
#include "alttestlib"

class JsonMapper {
public:
    static Json::Value binPackageToJson(const Alt::BinPackage& package);

    static Json::Value packageListToJson(const Alt::PackageList& packageList);

    static Json::Value archPackMapToJson(const Alt::ArchPackMap& archPackMap);

    static Json::Value branchPacksDiffToJson(const Alt::BranchPacksDiff& diff);

    static Json::Value targetDiffToJson(const Alt::TargetDiff& targetDiff);
};
#endif //ALT_TEST_JSONMAPPER_HPP
