//
// Created by asgrim on 24.05.24.
//

#ifndef ALT_TEST_ARRANGER_HPP
#define ALT_TEST_ARRANGER_HPP

#include <string>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

#include "apiClient.hpp"
#include "data.hpp"

namespace Alt{

    bool version_greater(const BinPackage &p1, const BinPackage &p2);

    //1 key = arch name
    //2 key = name of package
    typedef std::unordered_map<std::string_view, std::unordered_map<std::string_view,BinPackage>> ArchSearchMap;

    class BranchPackagesArranger {

    public:
        explicit BranchPackagesArranger(std::string mApiLink);

        BranchPacksDiff findBranchUniqPacks(std::string_view originBranch, std::string_view comparedBranch);

        TargetDiff getTargetDiff(std::string_view originBranch, std::string_view comparedBranch);

        template<typename Compare>
        BranchPacksDiff branchesInteresection(
                std::string_view originBranch,
                std::string_view comparedBranch,
                Compare compare = version_greater) {

            std::string originEndPoint = m_apiLink;
            originEndPoint.append(originBranch);

            std::string comparedEndPoint = m_apiLink;
            comparedEndPoint.append(comparedBranch);

            std::string json = m_apiClient.getRequest(originEndPoint);
            PackageList originList = parseJsonToPackageList(json);

            json = m_apiClient.getRequest(comparedEndPoint);
            PackageList comparedList = parseJsonToPackageList(json);


            Alt::BranchPacksDiff diff = findIntersection(makeSearchMap(originList), makeSearchMap(comparedList), compare);
            return diff;
        }

    private:

        PackageList parseJsonToPackageList(std::string_view json);
        BinPackage parseJsonToPackage(const Json::Value &value);

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

        std::string m_apiLink;
        details::ApiClient m_apiClient;
    };


}


#endif //ALT_TEST_ARRANGER_HPP
