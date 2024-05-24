//
// Created by asgrim on 24.05.24.
//

#ifndef ALT_TEST_CLIAPP_HPP
#define ALT_TEST_CLIAPP_HPP

#include <alttestlib>
#include <unordered_set>

class CliApp {
public:
    explicit CliApp(const std::string &apiLink);
    void addBranch(const std::string &s);
    void validate(int argc, char* argv[]);
    std::string arrangeBranchPackages(std::string_view origin, std::string_view compared);
private:
    std::string branchesToString();

    std::string m_apiLink;
    std::unordered_set<std::string> m_branches;
    Alt::BranchPackagesArranger m_arranger;
};
#endif //ALT_TEST_CLIAPP_HPP
