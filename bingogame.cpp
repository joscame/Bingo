#include "bingogame.h"
#include <iostream>
#include <vector>
#include "carton.h"
#include <algorithm>
#include <random>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "singing_square.h"

BingoGame::BingoGame(size_t cartones, const char *fill_words)
	:numero_cartones(cartones)
	,words(75)
{
	if (strcmp(fill_words, "") == 0)
	{
		for (int i = 1; i < 76; ++i)
			words[i - 1] = std::to_string(i);
	}
	else
	{
		crear_vector(fill_words);
		nums = false;
	}
	//std::cerr << "se crea el bingo todo tranquilo" << std::endl;
}

BingoGame::BingoGame(const char* fill_words)
	:numero_cartones(0)
	,words(75)
{
	crear_vector(fill_words);
	nums = false;
}

int BingoGame::card_shuffle()
{
	std::vector < std::string > temp(15);

	int contador = 0;
	while (contador < 4)
	{

		for (int i = 0; i < 15; ++i)
			temp[i] = words[i + (contador * 15)];

		vect_shuffle(temp);

		for (int i = 0; i < 15; ++i)
			words[i + (contador * 15)] = temp[i];

		++contador;
	}
	//std::cerr << "termino card shuffle" << std::endl;
	return 0;
}

bool BingoGame::vect_shuffle(std::vector < std::string >& temp)
{
	auto engine = std::default_random_engine{};
	std::shuffle(std::begin(temp), std::end(temp), engine);
	//std::cerr << "se usó el shuffle" << std::endl;
	return true;
}

bool BingoGame::crear_vector(const char* fill_words)
{
	std::string str;
	std::ifstream file;
	file.open(fill_words);
	if ( ! file )
	{
		std::cerr << "toc: could not open document (falla al crear vector)" << std::endl;
		return false;
	}
	int i = 0;
	while ( std::getline(file, str) && i < 75)
	{
		words[i] = str;
		++i;
	}
	file.close();
	//std::cerr << "se creó vector" << std::endl;
	return true;
}

bool BingoGame::imprimir_cartones(const char* template_name)
{
	std::string str;
	std::ofstream index_file;
	std::string temp_name(template_name);
	name = temp_name.substr(0, temp_name.length() - 4);
	index_file.open((name + ".idx").c_str());
	if ( ! index_file )
	{
		std::cerr << "toc: could not create index document" << std::endl;
		return false;
	}
	index_file << numero_cartones << std::endl;
	std::vector < std::string > temp(25);
    for (size_t i = 0; i < numero_cartones ; ++i)
	{
		card_shuffle();
		str = "";
		str = name + "-" + std::to_string(i) + ".svg";
		Carton nuevo(words);
		if ( ! nuevo.imprimir_carta(template_name, str.c_str(), i) )
		{
			std::cerr << "toc: could not print document(clase bingo)" << std::endl;
			return false;
		}
		nuevo.copiar_vector(temp);
		index_file << std::to_string(i) << std::endl;
		for (int j = 0; j < 25; ++j)
			index_file << temp[j] << std::endl;
		index_file << std::endl;
	}
	index_file.close();
	//std::cerr << "se imprimio carton" << std::endl;
	return true;
}

bool BingoGame::crear_archivo_fichas(const char* callingcard_name, const char* texts_name)
{
	std::string name("");
	std::string texts(texts_name);
	name +=  texts.substr(0, texts.length() - 4);
	name += "-";
	name += callingcard_name;
	std::ifstream game_texts_template;
	game_texts_template.open(callingcard_name);
	if ( ! game_texts_template )
	{
		std::cerr << "toc: could not open document" << std::endl;
		return false;
	}

	std::ofstream user_card;
	user_card.open(name.c_str());
	if ( ! user_card )
	{
		std::cerr << "toc: could not create document" << std::endl;
		return false;
	}
	std::string line;
	int i = 0;
	while (! game_texts_template.eof())
	{
		std::getline(game_texts_template, line);
        size_t pos = line.find("${");
		if ( pos != std::string::npos )
		{
			user_card << line.substr(0, pos) << words[i] << line.substr(pos + 9) << std::endl;
			++i;
		}
		else
		{
			user_card << line << std::endl;
		}

	}

	game_texts_template.close();
	user_card.close();
	return true;
}

bool BingoGame::sing(const char *symbols_file_name, const char* game_mode, int max_winners, const char* index_file_name)
{
	if (nums == true)
	{
		Singing_square square;
		std::cout << "Bingo call for symbols 1 through 75" << std::endl << "Press ENTER key to call next number" << std::endl << std::endl;
		square.print_square();

		std::string dummy;
		square.shuffle_index_numbers();
		while (std::getline (std::cin, dummy))
		{
			square.add_random_element();
			check_winners(index_file_name , game_mode, square);
			square.print_square();
            if (max_winners != 0)
                if (max_winners <= winners) return true;
		}
		return true;
	}
	else
	{
		Singing_square square(words);
		std::cout << "Bingo call for symbols " << symbols_file_name << std::endl << "Press ENTER key to call next symbol" << std::endl << std::endl;
		square.print_square();

		std::string dummy;
		square.shuffle_index_numbers();
		while (std::getline (std::cin, dummy))
		{
			square.add_random_element(words);
			check_winners(index_file_name, game_mode, square);
			square.print_square();
            if (max_winners != 0)
                if (max_winners <= winners) return true;
		}
		return true;
	}
    return false;
}

bool BingoGame::check_winners(const char *index_file_name, const char* game_mode, Singing_square &square)
{
	bool won = true;
    std::ifstream index_file;
    index_file.open(index_file_name);
    if ( ! index_file )
    {
		std::cerr << "toc: could not open document " << index_file_name << std::endl;
        return false;
    }
	std::string cantidad_cartones;
	std::getline (index_file, cantidad_cartones);
	numero_cartones = atoi(cantidad_cartones.c_str());
    for (size_t i = 0; i < numero_cartones; ++i)
    {
        std::string cart_number;
        std::getline (index_file, cart_number);
		won = check_carton(game_mode, index_file, square);
		if (won)
        {
			std::cout << "alguien gana" << std::endl;
            if(! revisar_ganador(i))
            {
                std::cout << "Winner " << winners + 1 << ": Cartón " << i <<  std::endl;
                the_winners[winners] = i;
                ++winners;
            }
        }
    }
	index_file.close();
    return true;
}

bool BingoGame::check_carton(const char* game_mode, std::ifstream& index_file, Singing_square &square)
{
	if (strcmp(game_mode, "four-corners") == 0)
		return check_four_corners(index_file, square);
	else if (strcmp(game_mode, "straight-line") == 0)
		return check_straight_line(index_file, square);
	else if (strcmp(game_mode, "diagonal") == 0)
		return check_diagonal(index_file, square);
	else if (strcmp(game_mode, "any-line") == 0)
		return check_any_line(index_file, square);
	else if (strcmp(game_mode, "roving-L") == 0)
		return check_roving_L(index_file, square);
	else
		return check_blackout(index_file, square);
}

bool BingoGame::check_four_corners(std::ifstream &index_file, Singing_square &square)
{
	if (square.get_size_of_singed_elements() < 4) return false;
	bool won = true;
	std::string element;
	for (int i = 0; i < 25; ++i)
	{
		std::getline (index_file, element);
		if (won && (i == 0 || i == 4 || i == 24 || i == 20))
		{
			if (! square.find_singed_element(element))
				won = false;
		}
	}
	std::getline (index_file, element); //esto es solo para brincarse el espacio en blanco que hay en el archivo destanciando cada carton
	return won;
}

bool BingoGame::check_straight_line(std::ifstream &index_file, Singing_square &square)
{
    if (square.get_size_of_singed_elements() < 5) return false;
    std::string element;
    bool col = true;
    bool won = false;
    std::vector < bool > rows = {true, true, true, true, true};
    for (int i = 0; i < 25; ++i)
    {
        std::getline (index_file, element);
        if (square.get_size_of_singed_elements() >= 5)
        {
            if (i % 5 == 0 && ! square.find_singed_element(element))
            {
                rows[0] = false;
                col = false;
            }
            else if (i % 4 == 0 && ! square.find_singed_element(element))
            {
                rows[4] == false;
                col = false;
            }
            else if ((i == 2 || i == 7 || i == 12 || i == 17 || i == 22) && ! square.find_singed_element(element))
            {
                rows[2] == false;
                col = false;
            }
            else if ((i == 1 || i == 6 || i == 11 || i == 16 || i == 21) && ! square.find_singed_element(element))
            {
                rows[1] == false;
                col = false;
            }
            else if ((i == 3 || i == 8 || i == 13 || i == 18 || i == 23) && ! square.find_singed_element(element))
            {
                rows[1] == false;
                col = false;
            }
            if(i % 5 == 0 && col)
                won = true;
            else
                col = true;
        }
    }
    std::getline (index_file, element); //esto es solo para brincarse el espacio en blanco que hay en el archivo destanciando cada carton
    if (square.get_size_of_singed_elements() >= 5)
    {
        if (won) return true;
        for (int j = 0; j <5; ++j)
            if (rows[j]) return true;
    }
    return false;

}

bool BingoGame::check_diagonal(std::ifstream &index_file, Singing_square &square)
{
    bool diagonal_izq = true;
    bool diagonal_der = true;
    std::string element;

    for (int i = 0; i < 25; ++i)
    {
        std::getline (index_file, element);
        if (i == 12 && ! square.find_singed_element(element))
        {
            diagonal_der = false;
            diagonal_izq = false;
        }
        else if ((i == 0 || i == 6 || i == 18 || i == 24) && ! square.find_singed_element(element))
            diagonal_izq = false;
        else if ((i == 20 || i == 16 || i == 8 || i == 4) && ! square.find_singed_element(element))
            diagonal_der = false;
    }
    std::getline (index_file, element);
    if (! diagonal_der && ! diagonal_izq)
        return false;
    else
        return true;
}

bool BingoGame::check_any_line(std::ifstream &index_file, Singing_square &square)
{
    if (square.get_size_of_singed_elements() >= 5)
    {
        std::vector < std::string > temp (25);
        int contador = 0;
        std::string element;
        bool diagonal_izq = true;
        bool diagonal_der = true;
        bool won = false;
        bool col = true;
        std::vector < bool > rows = {true, true, true, true, true};
        for (int i = 0; i < 25; ++i)
        {
            std::getline (index_file, element);
            temp[contador] = element;
            ++contador;
        }
        std::getline (index_file, element);

        if (! square.find_singed_element(temp[12]))
        {
            diagonal_der = false;
            diagonal_izq = false;
        }
        else
        {
            for (int i = 0; i < contador; ++i)
            {
                //para verticales y horizontales
                if (i % 5 == 0 && ! square.find_singed_element(temp[i]))
                {
                    rows[0] = false;
                    col = false;
                }
                else if ((i ==4 || i == 9 || i == 14 || i == 19 || i == 24) && ! square.find_singed_element(temp[i]))
                {
                    rows[4] == false;
                    col = false;
                }
                else if ((i == 2 || i == 7 || i == 12 || i == 17 || i == 22) && ! square.find_singed_element(temp[i]))
                {
                    rows[2] == false;
                    col = false;
                }
                else if ((i == 1 || i == 6 || i == 11 || i == 16 || i == 21) && ! square.find_singed_element(temp[i]))
                {
                    rows[1] == false;
                    col = false;
                }
                else if ((i == 3 || i == 8 || i == 13 || i == 18 || i == 23) && ! square.find_singed_element(temp[i]))
                {
                    rows[1] == false;
                    col = false;
                }
                if(i % 5 == 0 && col)
                    won = true;
                else
                    col = true;

                //para diagonales
                if ((i == 0 || i == 6 || i == 18 || i == 24) && ! square.find_singed_element(temp[i]) && diagonal_izq)
                    diagonal_izq = false;
                else if ((i == 20 || i == 16 || i == 8 || i == 4) && ! square.find_singed_element(temp[i]) && diagonal_der)
                    diagonal_der = false;
            }
        }
        if (diagonal_der || diagonal_izq || won)
            return true;
        else
        {
            for (int j = 0; j <5; ++j)
                if (rows[j]) return true;
            return false;
        }
    }
    return false;

}

bool BingoGame::check_roving_L(std::ifstream &index_file, Singing_square &square)
{
    if (square.get_size_of_singed_elements() >= 10)
    {
        bool colI = true;
        bool colD = true;
        bool rowU = true;
        bool rowD = true;
        std::string element;
        for (int i = 0; i < 25; ++i)
        {
            std::getline (index_file, element);
            if (i < 5 && ! square.find_singed_element(element))
                colI = false;
            if(i % 5 == 0 && ! square.find_singed_element(element))
                rowU = false;
            if ((i ==4 || i == 9 || i == 14 || i == 19 || i == 24) && ! square.find_singed_element(element))
                rowD = false;
            if (i >= 20 &&  ! square.find_singed_element(element))
                colD = false;
        }
        std::getline (index_file, element);
        if (! colI && ! colD)
            return false;
        else if (! rowU && ! rowD)
            return false;
        else
            return true;
    }
    return false;
}

bool BingoGame::check_blackout(std::ifstream &index_file, Singing_square &square)
{
    bool carton_lleno = true;
    std::string element;
    for (int i = 0; i < 25; ++i)
    {
        std::getline (index_file, element);
        if (! square.find_singed_element(element))
        carton_lleno = false;
    }
    std::getline (index_file, element);
    return carton_lleno;
}

//funcion que retorna falso si el ganador ingresado como parametro no se encuentra en la lista de ganadores actuales, y true si se encuentra en la lista
bool BingoGame::revisar_ganador(size_t the_winner)
{
    for (size_t i = 0; i < winners; ++i)
    {
        if (the_winners[i] == the_winner)
            return true;
    }
    return false;
}


