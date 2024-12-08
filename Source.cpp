#include <iostream>
#include <vector>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <tgbot/tgbot.h>
#include "APIManager.h"
#include "FrontManager.h"

const std::string KEY = "Your api key";

int main() {
	FrontManager* f_manager = new FrontManager(KEY);

	f_manager->start();
 }