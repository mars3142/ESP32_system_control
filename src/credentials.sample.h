#pragma once

#include <vector>
#include <string>

struct Credentials {
std::string ssid;
std::string password;

Credentials() = default;

Credentials(std::string ssid, std::string password) {
    this->ssid = ssid;
    this->password = password;
}
};

std::vector<Credentials> credentials { 
    Credentials("SSID", "SECRET_PASSWORD")
};
