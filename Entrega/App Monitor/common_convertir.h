//  
//  common_convertir.h
//  LIBRERIA CONVERTIR
//  
//  Librería de funciones conversoras.
//  


#ifndef CONVERTIR_H
#define CONVERTIR_H


#include <string.h>
#include <stdint.h>
#include <ctype.h>


/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Convertir {
public:

	// Devuelve el equivalente entero de un caracter hexadecimal
	static int htoi(char a);

	// Convierte un unsigned int a un string de contenido hexadecimal
	static std::string uitoh(uint8_t *a, size_t size);

	// Convierte un string de contenido hexadecimal a un unsigned int
	static uint8_t* htoui(const std::string& s);

	// Convierte un string en un integer
	static int stoi(const std::string& s);

	// Convierte un string en un unsigned integer
	static unsigned int stoui(const std::string& s);

	// Convierte un integer en un string
	static std::string itos(const int i);

	// Convierte un unsigned integer en un string
	static std::string uitos(const unsigned int i);

	// Convierte un string en un string en minusculas
	static std::string toLowercase(const std::string &s);
};


#endif
