//
// Created by asgrim on 24.05.24.
//

#ifndef ALT_TEST_DATA_HPP
#define ALT_TEST_DATA_HPP
#include <string>
#include <unordered_map>
#include <vector>

namespace Alt {
    struct BinPackage {
        std::string name;
        int64_t epoch;
        std::string version;
        std::string release;
        std::string arch;
        std::string disttag;
        int64_t buildtime;
        std::string source;
    };

    typedef std::vector<BinPackage> PackageList;

    //key = arch name
    //val = list of bin packages
    typedef std::unordered_map<std::string, PackageList> ArchPackMap;



    struct BranchPacksDiff {
        std::string branch1;
        std::string branch2;
        std::string difference;
        ArchPackMap comparison;
    };


    struct TargetDiff {
        BranchPacksDiff onlyBr1;
        BranchPacksDiff onlyBr2;
        BranchPacksDiff verHigherBr1;
    };
}
#endif //ALT_TEST_DATA_HPP
