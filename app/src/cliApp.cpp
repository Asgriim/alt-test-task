//
// Created by asgrim on 25.05.24.
//

#include <stdexcept>
#include "cliApp.hpp"
#include "jsonMapper.hpp"

CliApp::CliApp(const std::string &apiLink) : m_apiLink(apiLink), m_arranger(apiLink) {}

void CliApp::addBranch(const std::string &s) {
    m_branches.insert(s);
}

void CliApp::validate(int argc, char *argv[]) {
    if (argc != 3) {
        throw std::runtime_error("Argument number error: expected 3");
    }

    if (m_branches.find(argv[1]) == m_branches.end() || m_branches.find(argv[2]) == m_branches.end()) {
        throw std::runtime_error("Unknown branch error, available branches: " + branchesToString());
    }
}

std::string CliApp::branchesToString() {
    std::string s;
    for (auto &elem : m_branches) {
        s += elem + " ";
    }
    return s;
}

std::string CliApp::arrangeBranchPackages(std::string_view origin, std::string_view compared) {
    Alt::TargetDiff diff = m_arranger.getTargetDiff(origin, compared);
    Json::Value jsonTargetDiff = JsonMapper::targetDiffToJson(diff);

    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, jsonTargetDiff);

    return jsonString;
}

