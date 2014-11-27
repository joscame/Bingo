#ifndef CARTON_H
#define CARTON_H

#include <fstream>
#include <iostream>
#include <vector>
#include <cstdio>

class Carton
{

private:
	std::vector<std::string> tablero;

public: //constructores
	Carton(std::vector<std::string> palabras);

public: //member access
	bool imprimir_carta (const char *template_name, const char* file_name, int card_number);
	bool copiar_vector(std::vector<std::string>& copia);
};

#endif // CARTON_H
