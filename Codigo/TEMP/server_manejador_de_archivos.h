// Manejador de archivos 

#ifndef MANEJADOR_DE_ARCHIVOS_H
#define MANEJADOR_DE_ARCHIVOS_H

#include "common_mutex.h"
#include "common_lock.h"
#include "common_hash.h"
#include "common_lista.h"
#include "common_archivo.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>



namespace {
	// Constante para hacer referencia al archivo entero
	const std::string WHOLE_FILE = "FFFF";
}




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ManejadorDeArchivos {
private:

	std::string directorio;				// Directorio sobre el cual se trabaja
	Mutex m;							// Mutex

public:

	// Constructor
	ManejadorDeArchivos(const std::string& directorio);

	// Destructor
	~ManejadorDeArchivos();

	// Devuelve el contenido de un archivo en formato hexadecimal expresado
	// en una cadena de caracteres
	std::string obtenerContenidoArchivo(const std::string& nombre_archivo);

	// Devuelve un archivo que se localiza en el directorio
	// De no existir, devuelve un codigo de error = 0
	bool obtenerArchivo(const std::string &nombre_archivo, Archivo& archivo);

	// Elimina un archivo o un bloque de un archivo del directorio local
	// Devuelve 0 en caso de eliminar correctamente el archivo
	// Por ahora solamente borra archivos enteros, num_bloque = WHOLE_FILE
	int eliminarArchivo(const std::string &nombre_archivo, 
		const std::string &num_bloque);

	// Agrega un nuevo archivo o un bloque al directorio 
	// Por ahora solamente guarda archivos enteros, num_bloque = WHOLE_FILE
	// Devuelve el hash del archivo
	int agregarArchivo(const std::string &nombre_archivo, 
		const std::string &num_bloque, const std::string &bloque_archivo, std::string &hash);

	// Devuelve una lista con los archivos (ordenados por nombre) que se 
	// encuentran ubicados en el directorio administrado por el manejador.
	void obtenerArchivosDeDirectorio(Lista<Archivo>* listaArchivos);

	// Devuelve el hash del archivo con nombre especificado por parametros
	// De no existir, se devuelve un string vacio
	std::string obtenerHashArchivo(const std::string &nombre_archivo);
};

#endif
