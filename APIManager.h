#pragma once
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

class APIManager {
public:
	cpr::Response response;

	nlohmann::json json_response;

	nlohmann::json get_result(std::string url) {
		this->response = cpr::Get(cpr::Url{ url });
		if (this->response.status_code == 200) {
			this->json_response = nlohmann::json::parse(response.text);
			return this->json_response;
		}
		else {
			std::cout << "Error was occured: " << response.status_code;
			return nullptr;
		}
	}
};