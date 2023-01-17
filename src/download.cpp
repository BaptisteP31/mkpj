#include "../include/download.hpp"

#include <stdio.h>
#include <curl/curl.h>

// Curl url to path
void download_from_internet(std::string url, std::filesystem::path path) {
    CURL *curl;
    FILE *fp;

    curl = curl_easy_init();
    if (!curl)
        return;

    fp = fopen(path.c_str(), "wb");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(fp);
}