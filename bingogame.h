#ifndef BINGOGAME_H
#define BINGOGAME_H

#include <vector>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "singing_square.h"

class BingoGame
{

private:
	size_t numero_cartones;
	size_t contador_de_impresos = 0;
    size_t winners = 0;
	std::string name;
	std::vector < std::string > words;
    std::vector < size_t > the_winners;
	bool nums = true;

public:
	BingoGame(size_t cartones, const char* fill_words = "");
	BingoGame(const char *fill_words);

public:
	bool imprimir_cartones(const char *cstr);
	int card_shuffle();
	bool crear_vector(const char* fill_words);
	bool vect_shuffle(std::vector<std::string> &temp);
	bool crear_archivo_fichas(const char *callingcard_name, const char *texts_name);
	bool sing(const char* symbols_file_name, const char *game_mode, int max_winners, const char *index_file_name);
	bool check_winners(const char* index_file_name, const char *game_mode, Singing_square& square);
	bool check_carton(const char* game_mode, std::ifstream& index_file, Singing_square& square);
	bool check_four_corners(std::ifstream& index_file, Singing_square& square);
	bool check_straight_line(std::ifstream& index_file, Singing_square& square);
	bool check_diagonal(std::ifstream& index_file, Singing_square& square);
	bool check_any_line(std::ifstream& index_file, Singing_square& square);
	bool check_roving_L(std::ifstream& index_file, Singing_square& square);
    bool check_blackout(std::ifstream& index_file, Singing_square& square);
    bool revisar_ganador(size_t the_winner);
};

#endif // BINGOGAME_H
