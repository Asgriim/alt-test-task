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
Alt::BranchPackegesArranger::searchUnique(const Alt::ArchSearchMap &map1, const Alt::ArchSearchMap &map2) {
    Alt::BranchPacksDiff diff;
    for (auto &it1 : map1) {
    }
}
