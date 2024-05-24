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
    typedef std::unordered_map<std::string, PackageList> archPackMap;



    struct BranchPacksDiff {
        std::string arch;
        std::string branch1;
        std::string branch2;
        std::string difference;
        archPackMap comparison;
    };


    struct TargetDiff {
        BranchPacksDiff onlyBr1;
        BranchPacksDiff onlyBr2;
        BranchPacksDiff verHigherBr1;
    };

    //1 key = arch name
    //2 key = name of package
    typedef std::unordered_map<std::string_view, std::unordered_map<std::string_view,BinPackage>> ArchSearchMap;

    class BranchPackegesArranger {

    public:
        BranchPacksDiff findBranchUniqPacks(std::string_view originBranch, std::string_view comparedBranch);

        template<typename Compare>
        BranchPacksDiff branchesInteresection(std::string_view originBranch,
                                                           std::string_view comparedBranch,
                                                           Compare compare);
    private:


        BranchPacksDiff searchUnique(const ArchSearchMap &map1, const ArchSearchMap &map2);

        template<typename Compare>
        BranchPacksDiff findIntersection(const ArchSearchMap &map1, const ArchSearchMap &map2,
                                         Compare compare);
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
