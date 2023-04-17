#include <iostream>
#include <string>
#include <curl/curl.h>
#include <curl/easy.h>

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

    // Set libcurl options for TOR SOCKS5 proxy and target URL
    curl_easy_setopt(curl, CURLOPT_PROXY, "socks5h://127.0.0.1:9050");
    curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);
    if (argc >= 2 && std::string(argv[1]) == "-url") {
        curl_easy_setopt(curl, CURLOPT_URL, argv[2]);
    } else {
        std::cerr << "Please provide target URL with -url flag" << std::endl;
        return 1;
    }

    // Send NEWNYM command to TOR control port to get a new circuit
    CURLcode res = curl_easy_setopt(curl, CURLOPT_PROXY_TRANSFER_MODE, 1L);
    if (res != CURLE_OK) {
        std::cerr << "Failed to set transfer mode: " << curl_easy_strerror(res) << std::endl;
        return 1;
    }

    res = curl_easy_setopt(curl, CURLOPT_PROXYAUTH, (long)CURLAUTH_ANYSAFE);
    if (res != CURLE_OK) {
        std::cerr << "Failed to set proxy auth: " << curl_easy_strerror(res) << std::endl;
        return 1;
    }

    res = curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, "user:password");
    if (res != CURLE_OK) {
        std::cerr << "Failed to set proxy user:password: " << curl_easy_strerror(res) << std::endl;
        return 1;
    }

    const char* newnym_command = "signal/newnym\r\n";
    res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, newnym_command);
    if (res != CURLE_OK) {
        std::cerr << "Failed to set POSTFIELDS: " << curl_easy_strerror(res) << std::endl;
        return 1;
    }

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return 1;
    }

    // Perform the HTTP request and print the response
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return 1;
    }

    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}
