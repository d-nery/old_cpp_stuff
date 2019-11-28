#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace forcalegal {
	class WordDatabase {
	private:
		std::string fileName;
		std::vector<std::string> words;

	public:
		WordDatabase(std::string fileName);
		~WordDatabase();

		std::string getWord(unsigned int line) const;
		std::string getRandWord() const;

		bool hasWord(std::string word);

		unsigned int getNumPalavras() const;
		int removeWord(std::string word);
		int addWord(std::string word);

		void save();
	};

	std::ostream& operator<<(std::ostream& os, const WordDatabase& wdb);
}
