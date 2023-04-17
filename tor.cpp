#include <iostream>
#include <string>
#include <curl/curl.h>

int main(int argc, char* argv[])
{
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Set up the libcurl easy handle
    CURL* curl = curl_easy_init();
    if (curl == NULL) {
        std::cerr << "Failed to initialize curl handle" << std::endl;
        return 1;
    }

    // Set up the SOCKS5 proxy
    std::string proxyUrl = "socks5h://127.0.0.1:9050";
    curl_easy_setopt(curl, CURLOPT_PROXY, proxyUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);

    // Set libcurl options for target URL
    if (argc >= 2 && std::string(argv[1]) == "-url") {
        curl_easy_setopt(curl, CURLOPT_URL, argv[2]);
    } else {
        std::cerr << "Please provide target URL with -url flag" << std::endl;
        return 1;
    }

    // Perform the HTTP request and print the response
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return 1;
    }

    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}
