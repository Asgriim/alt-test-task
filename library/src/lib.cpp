//
// Created by asgrim on 24.05.24.
//

#include "lib.hpp"
#include <string_view>
#include <stdexcept>
#include <iostream>


Alt::ApiClient::ApiClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    m_curl = std::shared_ptr<CURL>(curl_easy_init(), curl_easy_cleanup);

    if (!m_curl) {
        throw std::runtime_error("Failed ti initialize CURL");
    }
}

Alt::ApiClient::~ApiClient() {
    curl_global_cleanup();
}

size_t Alt::ApiClient::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}



std::string Alt::ApiClient::getRequest(std::string_view endPoint) {
    std::string readBuffer;
    CURLcode res;
    if (m_curl) {
        curl_easy_setopt(m_curl.get(), CURLOPT_URL, endPoint.data());
        curl_easy_setopt(m_curl.get(), CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(m_curl.get(), CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(m_curl.get());

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
    }
    return std::move(readBuffer);
}





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

bool Alt::version_greater(const Alt::BinPackage &p1, const Alt::BinPackage &p2) {
    return p1.version > p2.version;
}
