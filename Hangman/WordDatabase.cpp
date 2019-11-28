#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>

#include "WordDatabase.hpp"

namespace forcalegal {
	WordDatabase::WordDatabase(std::string filename) : fileName(filename) {
		std::ifstream file(fileName);
		if (!file)
			throw std::runtime_error("Error opening file " + fileName);

		std::string tmp;
		while (getline(file, tmp))
			words.push_back(tmp);

		file.close();
	}

	WordDatabase::~WordDatabase() {
		save();
	}

	std::string WordDatabase::getWord(unsigned int line) const {
		if (line >= words.size()) return "";
		return words[line];
	}

	std::string WordDatabase::getRandWord() const {
		int i = rand() % words.size();
		return getWord(i);
	}

	unsigned int WordDatabase::getNumPalavras() const {
		return words.size();
	}

	bool WordDatabase::hasWord(std::string word) {
		for (int i = 0; i < words.size(); i++)
			if (word == words[i])
				return true;
		return false;
	}

	int WordDatabase::removeWord(std::string word) {
		if (!hasWord(word))
			return -1;
		for (int i = 0; i < words.size(); i++)
			if (word == words[i])
				words.erase(words.begin() + i);
		return 0;
	}

	int WordDatabase::addWord(std::string word) {
		if (hasWord(word))
			return -1;
		words.push_back(word);
		return 0;
	}

	void WordDatabase::save() {
		std::ofstream file(fileName, std::ofstream::out | std::ofstream::trunc);
		for (int i = 0; i < words.size(); i++)
			file << words[i] << std::endl;
	}

	std::ostream& operator<<(std::ostream& os, const WordDatabase& wdb) {
		for (int i = 0; i < wdb.getNumPalavras(); i++)
			os << wdb.getWord(i) << '\n';
		return os;
	}
}
