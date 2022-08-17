#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(void) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://tradestie.com/api/v1/apps/reddit");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        auto data = nlohmann::json::parse (readBuffer);
        std::cout << "Welcome to wsbshorts!" << std::endl;
        string tickertoshort = "no shorting for now";
        double sentiment = 0;
        for (int i = 0; i<data.size(); i++) {
            auto item = data[i];
            if (item["sentiment_score"] > sentiment) {
                sentiment = item["sentiment_score"];
                tickertoshort = item["ticker"];
            }
        }
        std::cout << "Right now you shoud short: "+tickertoshort << std::endl;
    }
    return 0;
}
