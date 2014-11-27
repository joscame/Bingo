#include "carton.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdio>

Carton::Carton(std::vector<std::string> palabras)
:tablero(25)
{
	size_t pos = 0;
	for (size_t col = 0; col < 5; ++col)
	{
		for (size_t fila = 0; fila < 5; ++fila)
		{
			tablero[pos] = palabras[ col * 15 + fila];
			++pos;
		}
	}
	//std::cerr << "se creo el carton todo tranquilo" << std::endl;
}

bool Carton::imprimir_carta(const char* template_name, const char* file_name, int card_number)
{
	std::ifstream plantilla;
	plantilla.open(template_name);
	if ( ! plantilla )
	{
		std::cerr << "toc: could not open document (falla al abrir plantilla)" << std::endl;
		return false;
	}
	//std::cerr << "abre la plantilla todo tranquilo" << std::endl;
	plantilla.clear();
	plantilla.seekg(0, std::ios_base::beg);


	std::ofstream carta;
	carta.open(file_name);
	if ( ! carta )
   {
   std::cerr << "toc: could not create document (clase carton)" << std::endl;
   return false;
   }
	//std::cerr << "se crea archivo de salida" << std::endl;
	std::string line;
	int i = 0;
	while (! plantilla.eof())
	{
		std::getline(plantilla, line);
		int pos = line.find("${");
		if ( pos != std::string::npos )
		{
			if(i == 25)
				carta << line.substr(0, pos) << std::to_string(card_number) << line.substr(pos + 12) << std::endl;
			else if (i == 12)
			{
				carta << line.substr(0, pos) << tablero[i] << line.substr(pos + 9) << std::endl;
				++i;
			}
			else
			{
				carta << line.substr(0, pos) << tablero[i] << line.substr(pos + 6) << std::endl;
				++i;
				//std::cerr << "graba en carta un numero->" << i << std::endl;
			}
		}
		else
		{
			carta << line << std::endl;
			//std::cerr << "graba en carta" << std::endl;
		}

	}

	plantilla.close();
	carta.close();
	//std::cerr << "se llena el archivo de salida" << std::endl;
	return true;
}

bool Carton::copiar_vector(std::vector<std::string> &copia)
{
	for (int i = 0; i < 25; ++i)
		copia[i] = tablero[i];
	return true;
}

