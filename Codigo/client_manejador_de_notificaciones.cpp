//
//  client_manejador_de_notificaciones.cpp
//  CLASE MANEJADORDENOTIFICACIONES
//  


#include "client_manejador_de_notificaciones.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
ManejadorDeNotificaciones::ManejadorDeNotificaciones(Receptor *receptor,
	Sincronizador *sincronizador, ReceptorDeArchivos *receptorDeArchivos) :
	receptor(receptor), sincronizador(sincronizador), 
	receptorDeArchivos(receptorDeArchivos) { }


// Destructor
ManejadorDeNotificaciones::~ManejadorDeNotificaciones() { }


// Define tareas a ejecutar en el hilo.
// Routea las notifiaciones y mensajes de entrada
void ManejadorDeNotificaciones::run() {
	std::string mensaje;
	while (this->isActive()) {
		// Pide un mensaje de la cola al receptor
		mensaje = this->receptor->obtenerMensajeDeEntrada();

		if (mensaje[0] == 'n')  { // Es una notificacion
			mensaje.erase(0,1);  // Se borra el identificador

			//DEBUG
			std::cout << "Es notificacion" << std::endl;
			//FIN DEBUG

			this->sincronizador->recibirNotificacion(mensaje);
		}
		else {
			if (mensaje[0] == 'f') {  // Es un archivo
				mensaje.erase(0,1);  // Se borra el identificador

				//DEBUG
				std::cout << "Es archivo" << std::endl;
				//FIN DEBUG

				this->receptorDeArchivos->recibirArchivo(mensaje);
			}
			else  // Con cualquier otro mensaje, frena
				this->stop();  
		}
		mensaje.clear();
	}
}

