#include <iostream>
#include <curl/curl.h>

// This is the callback function that is called by libcurl
size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string* response = (std::string*)userdata;
    response->append(ptr, size * nmemb);
    return size * nmemb;
}

// int main() {
//     CURL* curl = curl_easy_init();
//     std::string response;

//     // Set the URL to scrape
//     curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com");

//     // Set the callback function to handle the response
//     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

//     // Set the userdata parameter to a pointer to the response string
//     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

//     // Perform the request
//     CURLcode res = curl_easy_perform(curl);

//     // Check for errors
//     if (res != CURLE_OK) {
//         std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
//     }
//     else {
//         // Print the response
//         std::cout << response << std::endl;
//     }

//     // Clean up
//     curl_easy_cleanup(curl);

//     return 0;
// }


// int main() {
//   std::string url;
//   std::cout << "Enter the URL to scrape: ";
//   std::cin >> url;

//   CURL *curl;
//   CURLcode res;

//   curl = curl_easy_init();
//   if (curl) {
//     curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//     curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//     res = curl_easy_perform(curl);

//     if (res != CURLE_OK) {
//       std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
//     }

//     curl_easy_cleanup(curl);
//   }

//   return 0;
// }


int main() {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl" << std::endl;
        return 1;
    }

    std::string url;
    std::cout << "Enter target URL: ";
    std::getline(std::cin, url);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        std::cerr << "Failed to perform curl request: " << curl_easy_strerror(result) << std::endl;
        curl_easy_cleanup(curl);
        return 1;
    }

    std::cout << "Response: " << std::endl << response << std::endl;

    curl_easy_cleanup(curl);
    return 0;
}


// static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
// {
//     ((std::string*)userp)->append((char*)contents, size * nmemb);
//     return size * nmemb;
// }

// int main(int argc, char* argv[])
// {
//     std::string url;
//     for (int i = 1; i < argc; i++) {
//         if (std::string(argv[i]) == "-url") {
//             if (i + 1 < argc) {
//                 url = std::string(argv[i+1]);
//                 i++;
//             } else {
//                 std::cerr << "Error: missing URL after -url flag" << std::endl;
//                 return 1;
//             }
//         }
//     }
//     if (url.empty()) {
//         std::cerr << "Error: missing target URL" << std::endl;
//         return 1;
//     }

//     CURL *curl;
//     CURLcode res;
//     std::string readBuffer;

//     curl = curl_easy_init();
//     if (curl) {
//         curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//         curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
//         res = curl_easy_perform(curl);
//         if (res != CURLE_OK) {
//             std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;
//         } else {
//             std::cout << readBuffer << std::endl;
//         }
//         curl_easy_cleanup(curl);
//     } else {
//         std::cerr << "Error: failed to initialize CURL" << std::endl;
//     }
//     return 0;
// }
