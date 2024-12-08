#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <tgbot/tgbot.h>
#include "APIManager.h"


class FrontManager {
public:
	FrontManager(std::string KEY)
	{
		this->KEY = KEY;
	}

	void start() {
		TgBot::Bot bot(KEY);

		TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
		std::vector<TgBot::InlineKeyboardButton::Ptr> row0;
		TgBot::InlineKeyboardButton::Ptr oneButton(new TgBot::InlineKeyboardButton);
		oneButton->text = "1";
		oneButton->callbackData = "1";
		row0.push_back(oneButton);
		keyboard->inlineKeyboard.push_back(row0);

		bot.getEvents().onCommand("start", [&](TgBot::Message::Ptr message) {
			bot.getApi().sendMessage(message->chat->id, "1 - Get all pokemon\nname of pokemon - info of pokemon", 0, 0, keyboard);
		});

		bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
			if (StringTools::startsWith(message->text, "/start")) {
				return;
			}
			else if (StringTools::startsWith(message->text, "1")) {
				std::vector<std::string> names = this->getAllPokemons();
				for (int i = 0;i < names.size();i++) {
					bot.getApi().sendMessage(message->chat->id, names[i], 0, 0);
				}
			}
			else {
				std::cout << message->text;
				std::vector<std::string>info = this->getInfoByName(message->text);
				if (info.size() == 2) {
					bot.getApi().sendMessage(message->chat->id, info[0], 0, 0);
					bot.getApi().sendPhoto(message->chat->id, info[1], 0, 0);
				}
				else {
					bot.getApi().sendMessage(message->chat->id, "I don't know this pokemon", 0, 0);
				}
				std::cout << info[0] << "\n" + info[1] + "\n";
			}
		});

		try {
			TgBot::TgLongPoll longPoll(bot);
			while (true) {
				printf("Long polling\n");
				longPoll.start();
			}
		}
		catch (std::exception ex) {
			std::cout << ex.what();
		}
	}

	std::vector<std::string> getAllPokemons() {
		std::vector<std::string> names;
		std::string tmp = "";
		for (int i = 0;i < this->all_pokemons["count"];i++) {
			std::cout << i<<" ";
			if (tmp.size() <= 1000) {
				tmp += "/" + std::string(all_pokemons["results"][i]["name"]) + "\n";
			}
			else {
				names.push_back(tmp);
				tmp = "";
				tmp += "/" + std::string(all_pokemons["results"][i]["name"]) + "\n";
			}
		}
		return names;
	}

	std::string getInfoByName_tmp(std::string name) {
		std::string info = "";
		for (int i = 0;i < this->all_pokemons["count"];i++) {
			if (all_pokemons["results"][i]["name"] == name) {
				nlohmann::json pokemon = a_manager->get_result(all_pokemons["results"][i]["url"]);
				info += "Name: " + std::string(pokemon["name"]) + 
					"\nWeight: " + std::string(pokemon["weight"]) +
					"\nBase experience: " + std::string(pokemon["base_experience"]);
				for (auto i : pokemon["abilities"]) {
					info += "\nAbility: " + std::string(i["name"]);
				}
				return info;
			}
		}
		return "I don't know this pokemon\n";
	}

	std::vector<std::string> getInfoByName(std::string name) {
		std::vector<std::string> result;
		std::string info = "";
		nlohmann::json pokemon = a_manager->get_result("https://pokeapi.co/api/v2/pokemon"+name);
		std::cout << pokemon;
		if (pokemon) {
			info += "Name: " + std::string(pokemon["name"]) +
				"\nWeight: " + std::string(pokemon["weight"]) +
				"\nHeight: " + std::string(pokemon["height"]) +
				"\nBase experience: " + std::string(pokemon["base_experience"]) + "\n";
			result.push_back(info);
			result.push_back(std::string(pokemon["sprites"]["other"]["official-artwork"]["front_default"]));
		}
		return result;
	}

	std::vector<std::string> split(const std::string& str, char delimiter) {
		std::vector<std::string> tokens;
		size_t start = 0, end = 0;

		while ((end = str.find(delimiter, start)) != std::string::npos) {
			tokens.push_back(str.substr(start, end - start));
			start = end + 1;
		}
		tokens.push_back(str.substr(start));

		return tokens;
	}
private:
	std::string KEY;

	APIManager* a_manager = new APIManager();

	nlohmann::json all_pokemons = a_manager->get_result("https://pokeapi.co/api/v2/pokemon?offset=0&limit=1302");
};