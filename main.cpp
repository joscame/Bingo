#include <iostream>
#include <cstddef>
#include "carton.h"
#include "bingogame.h"
#include <cstring>
#include <cctype>

int print_help()
{
	std::cout << "\nCreate Bingo cards and simulate game sessions. Usage:\n"
				   <<"bingo generate NUMBER card.svg [-s symbols.txt] [-c callingcard.svg]\nbingo call [card.idx] [-t type] [-s symbols.txt] [-w winners]\n\nActions:\ngenerate               Generate random cards in current directory\n"
				   << "call                   Simulate a bingo game session\n\nOptions:\nNUMBER                 Number of cards to be generated\ncard.svg               Graphic design of the card with ${field} placeholders\n"
				   << "card.idx               Contains all the generated cards and their 24 or 25 symbols\n-c callingcard.svg     Fill out the given calling card with all 75 symbols\n"
				   << "-s symbols.txt         Use the given 75 symbols instead of numbers\n-t type                Pattern to be matched in order to win. Valid types:\n"
				   << "                       four-corners, straight-line, roving-L, blackout\n-w winners             Number of winners until finish the session\n" << std::endl;
	return 0;
}

int print_version()
{
	std::cout << "\nBingo v1.0 [2014-Nov-8] Jose Miguel Mesén Campos <josem6022@yahoo.com>"
		   "\n"
		   "This is free software distributed under Creative Commons Attribution 4.0\n"
		   "International (CC BY 4.0) license.\n\n" << std::endl;;

	return 0;
}

bool analyze_params(int argc, char* argv[])
{
	std::string words_file_name("");
	std::string callingcard("");
	std::string game_mode("");
	std::string index_name("");
    int max_winners = 0;
	bool create_texts = false;
	bool generate = false;
	bool call = false;
	int number = 0;
	std::string template_name("");
	for ( int i = 1; i < argc; ++i )
	{
		if ( strcmp(argv[i], "-s") == 0 )
			words_file_name = argv[i + 1];
        else if ( strcmp(argv[i], "-t") == 0 )
			game_mode = argv[i + 1];
        else if ( strcmp(argv[i], "-w") == 0 )
            max_winners = atoi (argv[i + 1]);
		else if ( strcmp(argv[i], "call") == 0)
		{
			call = true;
			index_name = argv[i + 1];
		}
		else if ( strcmp(argv[i], "--help") == 0)
			return print_help();
		else if ( strcmp(argv[i], "--version") == 0)
			return print_version();
		else if ( strcmp(argv[i], "generate") == 0)
			generate = true;
		else if ( strcmp(argv[i], "-c") == 0)
		{
			create_texts = true;
			callingcard = argv[i+1];
		}
		else if ( isdigit(*argv[i]) != 0)
			number = atoi(argv[i]);
		else if ( strcmp(argv[i - 1], "-s") != 0 && strcmp(argv[i - 1], "-c") != 0)
			template_name = argv[i];			
	}
	if (generate == true)
	{
		if (number != 0)
		{
			BingoGame nuevo(number, words_file_name.c_str());
			nuevo.imprimir_cartones(template_name.c_str());
			if (create_texts == true)
				return nuevo.crear_archivo_fichas(callingcard.c_str(), words_file_name.c_str());
		}
		else if (create_texts == true)
		{
			BingoGame nuevo(words_file_name.c_str());
			nuevo.crear_archivo_fichas(callingcard.c_str(), words_file_name.c_str());
		}
	}
	else if (call)
	{
		BingoGame nuevo(0, words_file_name.c_str());
		return nuevo.sing(words_file_name.c_str(), game_mode.c_str(), max_winners, index_name.c_str());
	}

	return true;
}

int main(int argc, char* argv[])
{
	if ( ! analyze_params(argc, argv) )
		return 1;
	return 0;
}
