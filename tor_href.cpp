#include <iostream>
#include <string>
#include <curl/curl.h>
#include "./dependencies/rapidxml.hpp"

// Callback function for libcurl to write HTTP response
size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* data)
{
    size_t real_size = size * nmemb;
    data->append(ptr, real_size);
    return real_size;
}

// Callback function for RapidXML to handle anchor tags
void anchor_callback(rapidxml::xml_node<>* node)
{
    rapidxml::xml_attribute<>* href_attr = node->first_attribute("href");
    if (href_attr != NULL) {
        std::cout << href_attr->value() << std::endl;
    }
}

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

    // Set libcurl option to use the write_callback function to capture the HTTP response
    std::string http_response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &http_response);

    // Perform the HTTP request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return 1;
    }

    // Parse the HTML response using RapidXML
    rapidxml::xml_document<> doc;
    doc.parse<0>(const_cast<char*>(http_response.c_str()));

    // Traverse the HTML tree and extract href links inside anchor tags
    rapidxml::xml_node<>* root = doc.first_node();
    for (rapidxml::xml_node<>* node = root->first_node(); node != NULL; node = node->next_sibling()) {
        if (std::string(node->name()) == "a") {
            anchor_callback(node);
        }
    }

    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}
