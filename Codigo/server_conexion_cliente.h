//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//  


#ifndef CONEXION_CLIENTE_H
#define CONEXION_CLIENTE_H



#include "common_thread.h"
#include "common_socket.h"
#include "common_comunicador.h"
#include "common_logger.h"
#include "server_receptor.h"
#include "server_recolector_de_informacion.h"
#include "server_administrador_de_cuentas.h"
class AdministradorDeClientes;



namespace {
	// Constantes para los nombres de directorio
	const std::string MONITOR_USER = "ADMMONITOR";
}





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ConexionCliente : public Thread {
private:

	Socket *socket;								// Socket de comunicación
	std::string nombreUsuario;					// Nombre de usuario de cliente
	AdministradorDeClientes *admClientes;		// Administrador de clientes
	AdministradorDeCuentas* admCuentas;					// Administrador cuentas de cliente
	Receptor *receptor;							// Receptor a donde se envian
	std::string pathCarpeta;					// Path donde se encuentran
												// los archivos del cliente
												// los datos que arrivan
	bool habilitarRecepcion;					// Traba para evitar recepción
												// hasta que se indique.
	Logger *logger;								// Logger de eventos
	std::string clave;							// Clave utilizada para firmar 
												// mensajes

	// Espera inicio sesion
	int inicioSesion(Comunicador& comunicador);

	// Se ocupa de atender a las solicitudes enviadas por el monitor.
	void atenderAMonitor(std::string& mensaje, Comunicador *com);

public:

	// Constructor
	// PRE: 's' es un socket para la comunicación con el cliente; 'id' es 
	// número de cliente que se le ha sido asignado por el servidor; 'serv' es
	// una referencia al servidor al que pertenece la conexión.
	ConexionCliente(Socket *s, AdministradorDeClientes *adm, AdministradorDeCuentas *ac,
		Logger *logger);

	// Destructor
	~ConexionCliente();

	// Devuelve el id que identifica a la conexión.
	int id();

	// Define tareas a ejecutar en el hilo.
	virtual void run();

	// Detiene la conexión con el cliente. No debe utilizarse el método stop()
	// para detener, sino este mismo en su lugar.
	void detener();

	// Devuelve el nombre de usuario con el que inicio sesión el cliente.
	// POST: si aún no ha iniciado sesión, se devuelve una cadena vacía.
	std::string getNombreUsuario();

	// Asigna un receptor a la conexión, a quien le enviará los datos que se
	// reciban del cliente.
	// PRE: 'unReceptor' es el receptor.
	// POST: la conexión comenzará a derivar los datos llegados hacia el
	// receptor.
	void asignarReceptor(Receptor *unReceptor);

	// Envia un mensaje al cliente.
	// PRE: 'mensaje' es la cadena que desea enviarse.
	// POST: lanza una excepción si el socket no se encuentra activo.
	void enviarMensaje(std::string& mensaje);
};

#endif
