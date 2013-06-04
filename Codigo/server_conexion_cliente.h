//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//  


#ifndef CONEXION_CLIENTE_H
#define CONEXION_CLIENTE_H



#include "common_thread.h"
#include "common_socket.h"
#include "common_comunicador.h"
#include "server_verificador.h"





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ConexionCliente : public Thread {
private:

	Socket *socket;								// Socket de comunicación
	Verificador* verificador;					// Verificador de loggin

	// Espera inicio sesion
	int inicioSesion(Comunicador& comunicador);

public:

	// Constructor
	// PRE: 's' es un socket para la comunicación con el cliente; 'id' es 
	// número de cliente que se le ha sido asignado por el servidor; 'serv' es
	// una referencia al servidor al que pertenece la conexión.
	//ConexionCliente(Socket *s, ControladorDeTareas *controlador);
	ConexionCliente(Socket *s, Verificador* v);

	// Destructor
	~ConexionCliente();

	// Define tareas a ejecutar en el hilo.
	virtual void run();

	// Detiene la conexión con el cliente. No debe utilizarse el método stop()
	// para detener, sino este mismo en su lugar.
	void detener();
};

#endif
