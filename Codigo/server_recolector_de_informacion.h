// Devuelve informacion sobre cantidad de bytes almacenados en servidor

#ifndef SERVER_RECOLECTOR_DE_INFORMACION_H
#define SERVER_RECOLECTOR_DE_INFORMACION_H

#include <string>
#include <stack>
#include "dirent.h"
#include <sys/stat.h>
#include "server_configuracion.h"

class Recolector {
public:

	// Devuelve la cantidad de bytes almacenados en el servidor
	// recorriendo todos los directorios que se encuentran a partir
	// del raiz. Soporta directorios anidados
	static int cantidadBytesAlmacenados();
};

#endif /* SERVER_RECOLECTOR_DE_INFORMACION_H */
