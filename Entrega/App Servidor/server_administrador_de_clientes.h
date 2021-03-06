//  
//  server_administrador_de_clientes.h
//  CLASE ADMINISTRADORDECLIENTES
//  


#ifndef ADMINISTRADOR_DE_CLIENTES_H
#define ADMINISTRADOR_DE_CLIENTES_H


#include <string>
#include <map>
#include "common_thread.h"
#include "common_cola.h"
#include "common_lista.h"
#include "common_logger.h"
#include "server_conexion_cliente.h"
#include "server_carpeta.h"





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class AdministradorDeClientes : public Thread {
private:

	std::map< std::string, Carpeta* > carpetas;		// Diccionario de carpetas
	Lista< ConexionCliente* > listaMonitores;		// Lista de monitores
	Cola< ConexionCliente* > conexionesMuertas;		// Cola de conexiones que
													// deben ser destruidas
	Logger *logger;									// Logger de eventos

public:

	// Constructor
	AdministradorDeClientes(Logger *logger);

	// Destructor
	~AdministradorDeClientes();

	// Ingresa un cliente como miembro activo del directorio al que se
	// encuentra vinculado.
	void ingresarCliente(std::string usuario, ConexionCliente *unCliente, 
		const std::string &pathCarpeta, const std::string &clave);

	// Da de baja a un cliente, el cual debe haber sido ingresado previamente
	// como miembro activo del directorio al que se encuentra vinculado.
	void darDeBajaCliente(std::string usuario, ConexionCliente *unCliente);

	// Da aviso al administrador de que debe destruirse un cliente
	void destruirCliente(ConexionCliente *unCLiente);

	// Define tareas a ejecutar en el hilo.
	virtual void run();

	// Inicia el administrador de clientes
	void iniciar();

	// Detiene el administrador de clientes
	void detener();

	// Devuelve la cantidad de clientes conectados actualmente.
	unsigned int cantidadDeClientesConectados();

	// Devuelve la cantidad de carpetas activas actualmente.
	unsigned int cantidadDeCarpetasActivas();
};

#endif
