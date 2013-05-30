//
//  client_emisor.h
//  CLASE EMISOR
//  


#ifndef EMISOR_H
#define EMISOR_H

#include <string>
#include "common_thread.h"
#include "common_cola.h"
class Socket;





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Emisor : public Thread {
private:

	Socket socket;							// Socket por el que envía datos
	Cola< std::string > salida;				// Cola de salida

public:

	// Constructor
	Emisor();

	// Destructor
	~Emisor();

	// Agrega un mensaje a enviar en la cola de salida del emisor.
	// PRE: 'mensaje' es la cadena de texto que se desea enviar
	void ingresarMensajeDeSalida(std::string mensaje);

	// Define tareas a ejecutar en el hilo.
	// Se encarga de emitir lo que se encuentre en la cola de salida.
	virtual void run();
};

#endif
