//
// Created by asgrim on 24.05.24.
//

#include <iostream>
#include "cliApp.hpp"

int main(int argc, char* argv[]) {
    const std::string apiLink = "https://rdb.altlinux.org/api/export/branch_binary_packages/";
    CliApp app(apiLink);
    app.addBranch("sisyphus");
    app.addBranch("p9");
    app.addBranch("p10");
    app.validate(argc, argv);
    std::cout << app.arrangeBranchPackages(argv[1], argv[2]);

    return 0;
}