#include "singing_square.h"
#include <vector>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <random>
#include <algorithm>

Singing_square::Singing_square()
	:square(75)
	,index_numbers(75)
    ,temp_elem_singed(75)
{
	for (int i = 0; i < 75; ++i)
		square[i] = "--";
	for (int i = 1; i < 76; ++i)
		index_numbers[i - 1] = std::to_string(i);
}

Singing_square::Singing_square(const std::vector<std::string> &vec)
	:square(75)
	,index_numbers(75)
    ,temp_elem_singed(75)
{
	for (int i = 0; i < 75; ++i)
	{
		int len = vec[i].length();
		if ( len > max_word_len )
			max_word_len = len;
	}
	std::string fill_chars("");
	for (int i = 0; i < max_word_len; ++i)
		fill_chars += "-";
	for (int i = 0; i < 75; ++i)
		square[i] = fill_chars;
	for (int i = 1; i < 76; ++i)
		index_numbers[i - 1] = std::to_string(i);
	numeric_square = false;
}

bool Singing_square::add(std::string nuevo, int pos)
{
	int num_spaces = max_word_len - nuevo.length();
	for (int i = 0; i < num_spaces; ++i)
		nuevo += "-";
	square[pos] = nuevo;
    temp_elem_singed[elements_singed] = nuevo;
    ++elements_singed;
	return true;
}

bool Singing_square::add_random_element()
{
	if (contador_turnos == 75)
	{
		std::cerr << "ya no hay mas elementos que cantar" << std::endl;
		return false;
	}
	add (index_numbers[contador_turnos], atoi (index_numbers[contador_turnos].c_str()) - 1);
	std::cout << "NUMERO:  " << index_numbers[contador_turnos] << std::endl;
	++contador_turnos;
	return true;
}

bool Singing_square::add_random_element(std::vector<std::string> &source_vector)
{
	if (contador_turnos == 75)
	{
		std::cerr << "ya no hay mas elementos que cantar" << std::endl;
		return false;
	}
	add (source_vector[atoi (index_numbers[contador_turnos].c_str()) - 1], atoi (index_numbers[contador_turnos].c_str()) - 1);
	std::cout << "PALABRA:  " << source_vector[atoi (index_numbers[contador_turnos].c_str()) - 1] << std::endl;
	++contador_turnos;
	return true;
}

bool Singing_square::print_square()
{
	std::cout << std::endl << "BINGO" << std::endl;
	if (numeric_square)
	{
		for (int i = 0; i < 75; ++i)
		{
			if (i % 15 == 0 && i != 0)
				std::cout << std::endl;
			std::cout << square[i] << " ";
		}
		std::cout << std::endl << std::endl;
		return true;
	}
	else
	{
		for (int i = 0; i < 75; ++i)
		{
			if (i % 5 == 0)
				std::cout << std::endl;
			std::cout << square[i] << " ";
		}
		std::cout << std::endl << std::endl;
		return true;
	}
	return false;
}

bool Singing_square::shuffle_index_numbers()
{
	auto engine = std::default_random_engine{};
	std::shuffle(std::begin(index_numbers), std::end(index_numbers), engine);
	return true;
}

bool Singing_square::find_singed_element(std::string& element)
{
	int i = 0;
	while (i < elements_singed)
	{
		if (element == temp_elem_singed[i]) return true;
		++i;
	}
	return false;
}

