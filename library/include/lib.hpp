//
// Created by asgrim on 24.05.24.
//

#ifndef ALT_TEST_LIB_HPP
#define ALT_TEST_LIB_HPP
#include <string>
#include <curl/curl.h>
#include <memory>
#include <vector>
#include <unordered_map>

namespace Alt{

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
        std::string arch;
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

    bool version_greater(const BinPackage &p1, const BinPackage &p2);

    //1 key = arch name
    //2 key = name of package
    typedef std::unordered_map<std::string_view, std::unordered_map<std::string_view,BinPackage>> ArchSearchMap;

    class BranchPackagesArranger {

    public:
        BranchPacksDiff findBranchUniqPacks(std::string_view originBranch, std::string_view comparedBranch);

        template<typename Compare>
        BranchPacksDiff branchesInteresection(std::string_view originBranch,
                                                           std::string_view comparedBranch,
                                                           Compare compare = version_greater);

        ArchSearchMap makeSearchMap(const PackageList &list);

        BranchPacksDiff searchUnique(const ArchSearchMap &map1, const ArchSearchMap &map2);
        bool contains(std::string_view arch, std::string_view name, const ArchSearchMap &map);


        template<typename Compare = decltype(version_greater)>
        BranchPacksDiff findIntersection(const ArchSearchMap &map1,
                                         const ArchSearchMap &map2,
                                         Compare compare = version_greater) {
            Alt::BranchPacksDiff diff;
            for (auto &archIt : map1) {
                std::string_view archName = archIt.first;
                for (auto &nameIt : archIt.second) {
                    std::string_view pacName = nameIt.first;

                    if (contains(archName, pacName, map2)) {
                        const BinPackage &p2 = map2.find(archName)->second.find(pacName)->second;

                        if (compare(nameIt.second, p2)){
                            diff.comparison[archName.data()].push_back(nameIt.second);
                        }

                    }
                }
            }
            return std::move(diff);
        }

    };

    class ApiClient {
    public:
        ApiClient();

        virtual ~ApiClient();

        std::string getRequest(std::string_view endPoint);

    private:
        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

        std::shared_ptr<CURL> m_curl;
    };


}


#endif //ALT_TEST_LIB_HPP
