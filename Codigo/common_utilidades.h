// Funciones necesarias

#ifndef COMMON_UTILIDADES_H
#define COMMON_UTILIDADES_H

#include <string>
#include <time.h>
#include <cstdlib>

class Utilidades {
public:

	// Devuelve un string de caracteres alfanumericos aleatorios de 
	// tamanio 'longitud'
	static void randomString(std::string &s, int longitud);
};


#endif /* COMMON_UTILIDADES_H */
