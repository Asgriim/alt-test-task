//
// Created by asgrim on 24.05.24.
//

#ifndef ALT_TEST_APICLIENT_HPP
#define ALT_TEST_APICLIENT_HPP

#include <memory>
#include <curl/curl.h>
#include <string_view>

namespace details {
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
#endif //ALT_TEST_APICLIENT_HPP
