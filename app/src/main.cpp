//
// Created by asgrim on 24.05.24.
//
#include "lib.hpp"
#include <iostream>
int main() {
    Alt::ApiClient apiClient;
    std::string resp = apiClient.getRequest("https://rdb.altlinux.org/api/version");
    std::cout << resp;
    return 0;
}