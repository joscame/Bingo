#ifndef SINGING_SQUARE_H
#define SINGING_SQUARE_H

#include <vector>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <random>
#include <algorithm>

class Singing_square
{
private:
	std::vector < std::string > square;
	std::vector < std::string > index_numbers;
    std::vector < std::string > temp_elem_singed;
	bool numeric_square = true;
	int contador_turnos = 0;
    int max_word_len = 2;
    int elements_singed = 0;
public:
	Singing_square();
	Singing_square( const std::vector < std::string >& vec );

public:
	bool add(std::string nuevo, int pos);
	bool add_random_element(std::vector < std::string >& source_vector);
	bool add_random_element();
	bool print_square();
	bool shuffle_index_numbers();
    inline const std::string& get_singed_element(size_t element_index){return temp_elem_singed[element_index];}
	inline int get_size_of_singed_elements(){return elements_singed;}
	bool find_singed_element(std::string &element);
};

#endif // SINGING_SQUARE_H
