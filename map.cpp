#include "map.h"

Map::Map() {
}

bool Map::add(const std::string & word) {
	if (word_id_map.count(word) == 0) {
	    int number_of_word = size();
		word_id_map[word] = number_of_word;
		word_by_id.push_back(word);
	}
	return true;
}

int Map::size() const {
	return static_cast<int>(word_id_map.size());
}

int Map::id(const std::string & word) const {
	return (word_id_map.end() == word_id_map.find(word)) ? -1 : (*word_id_map.find(word)).second;
}

std::string Map::word(int id) const {
	return word_by_id[id];
}
