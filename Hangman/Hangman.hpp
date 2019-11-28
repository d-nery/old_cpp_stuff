#pragma once

#include <vector>
#include <string>

#include <SFML/Audio.hpp>

#include "WordDatabase.hpp"

namespace forcalegal {
	class Hangman {
	private:
		WordDatabase& database;
		unsigned int chances;
		unsigned int initialChances;
		unsigned int _try;
		sf::Music audios[3];

		std::string word;
		std::string masked;
		std::string mesage;
		std::string tries[4];

	public:
		Hangman(WordDatabase& database, unsigned int chances = 6);
		~Hangman();

		void maskWord();
		void resetTries();
		void showScreen();

		int getTry();

		bool won();
	};
}
