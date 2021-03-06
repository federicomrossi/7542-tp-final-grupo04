//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//   

#include <iostream>
#include <sstream>
#include "common_protocolo.h"
#include "common_parser.h"
#include "common_convertir.h"
#include "server_administrador_de_clientes.h"
#include "server_conexion_cliente.h"
#include "common_lista.h"





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
// PRE: 's' es un socket para la comunicación con el cliente; 'id' es 
// número de cliente que se le ha sido asignado por el servidor; 'serv' es
// una referencia al servidor al que pertenece la conexión.
ConexionCliente::ConexionCliente(Socket *s,
	AdministradorDeClientes *adm, AdministradorDeCuentas *ac, Logger *logger) : 
	socket(s), nombreUsuario(""), admClientes(adm), admCuentas(ac), 
	pathCarpeta(""), habilitarRecepcion(false), logger(logger) { }


// Destructor
ConexionCliente::~ConexionCliente() { 
	// Liberamos memoria utilizada por el socket
	delete this->socket;
}


// Devuelve el id que identifica a la conexión.
int ConexionCliente::id() {
	return this->socket->obtenerID();
}


// Define tareas a ejecutar en el hilo.
void ConexionCliente::run() {
	// Creamos el comunicador para recibir mensajes
	Comunicador comunicador(this->socket);

	// Variables de procesamiento
	std::string mensaje;

	// Mensaje de log
	this->logger->emitirLog("LOGIN: Esperando datos de login del cliente...");

	// Si el inicio de sesión falló, cerramos conexión con cliente
	if(this->inicioSesion(comunicador) != 1) {
		this->admClientes->destruirCliente(this);

		// Mensaje de log
		this->logger->emitirLog("LOGIN: Falló inicio de sesión de cliente.");
		this->logger->emitirLog("Se ha desconectado al cliente.");

		return;
	}

	// Mensaje de log
	this->logger->emitirLog("LOGIN: Usuario '" + this->nombreUsuario + "'" 
		+ " ha iniciado sesión.");

	// Nos autoregistramos en el administrador de clientes
	this->admClientes->ingresarCliente(this->nombreUsuario, this, 
		this->pathCarpeta, this->clave);

	// Si se conectó una aplicación monitor, derivamos hacia allí
	if(this->nombreUsuario == MONITOR_USER) {
		// Se inicia la recepción de mensajes desde el cliente
		while(this->isActive()) {
			// Esperamos a recibir mensaje
			if(comunicador.recibir(mensaje) == -1) break;

			// Derivamos para que sea atendido
			this->atenderAMonitor(mensaje, &comunicador);
		}
	}
	else {
		// Esperamos a que se habilite la recepción, es decir, que se 
		// especifique un receptor
		while(!habilitarRecepcion) 
			if(!this->isActive()) return;

		// Se inicia la recepción de mensajes desde el cliente
		while(this->isActive()) {
			// Esperamos a recibir mensaje
			if(comunicador.recibir(mensaje) == -1) break;

			// Enviamos el mensaje al receptor
			this->receptor->ingresarMensajeDeEntrada(this->id(), mensaje);
		}
	}
	
	// Avisamos al administrador que la conexión debe darse de baja y ser
	// destruida
	this->admClientes->darDeBajaCliente(this->nombreUsuario, this);
	this->admClientes->destruirCliente(this);

	// Mensaje de log
	this->logger->emitirLog("Usuario '" + this->nombreUsuario + "' " 
		+ " se ha desconectado.");
}


// Detiene la conexión con el cliente. No debe utilizarse el método stop()
// para detener, sino este mismo en su lugar.
void ConexionCliente::detener() {
	// Detenemos hilo
	this->stop();
	
	// Forzamos el cierre del socket y destrabamos espera de recepcion de datos
	try {
		this->socket->cerrar();
	}
	// Ante una eventual detención abrupta, previa a la inicialización del
	// socket, lanzará un error que daremos por obviado.
	catch(...) { } 
}


// Devuelve el nombre de usuario con el que inicio sesión el cliente.
// POST: si aún no ha iniciado sesión, se devuelve una cadena vacía.
std::string ConexionCliente::getNombreUsuario() {
	return this->nombreUsuario;
}


// Asigna un receptor a la conexión, a quien le enviará los datos que se
// reciban del cliente.
// PRE: 'unReceptor' es el receptor.
// POST: la conexión comenzará a derivar los datos llegados hacia el
// receptor.
void ConexionCliente::asignarReceptor(Receptor *unReceptor) {
	this->receptor = unReceptor;

	// Habilitamos la recepción de datos
	this->habilitarRecepcion = true;
}


// Envia un mensaje al cliente.
// PRE: 'mensaje' es la cadena que desea enviarse.
// POST: lanza una excepción si el socket no se encuentra activo.
void ConexionCliente::enviarMensaje(std::string& mensaje) {
	// Corroboramos que el socket esté activo
	if(!this->socket->estaActivo()) {
		// Mensaje de log
		this->logger->emitirLog("ERROR: No se pudo emitir mensaje a usuario '"
			+ this->nombreUsuario + "'");

		// Lanzamos excepción
		throw "ERROR: No se pudo emitir mensaje a cliente.";
	}

	// Creamos el comunicador para enviar mensajes
	Comunicador comunicador(this->socket);

	// Enviamos el mensaje
	comunicador.emitir(mensaje);
}





/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */
 

// Espera inicio sesion
int ConexionCliente::inicioSesion(Comunicador& comunicador) {
	// Variables de procesamiento
	std::string instruccion;
	std::string args;
	
	// Se recibe la instruccion
	if(comunicador.recibir(instruccion, args) == -1) 
		return -1;

	// Se debe crear nuevo usuario
	if (instruccion == C_LOGIN_REQUEST) {
		// Caso en que la verificación es correcta
		if (admCuentas->verificarCliente(args, this->nombreUsuario, 
			this->pathCarpeta, this->clave) == 1) {  
			comunicador.emitir(S_LOGIN_OK);
			return 1;
		}
		// Caso en que la verificación es incorrecta
		else {
			comunicador.emitir(S_LOGIN_FAIL);
			return 0;
		}
	}
	return -1;
}


// Se ocupa de atender a las solicitudes enviadas por el monitor.
void ConexionCliente::atenderAMonitor(std::string& mensaje, Comunicador *com) {
	// Variables auxiliares
	std::string instruccion, args;
	AdministradorDeCuentas admin;

	// Parseamos instruccion
	Parser::parserInstruccion(mensaje, instruccion, args);

	// Caso en que se solicita información al server
	if(instruccion == M_SERVER_INFO_REQUEST) {
		// Armamos respuesta
		std::string respuesta;
		respuesta.append(S_SERVER_INFO);
		respuesta.append(" ");
		respuesta.append(Convertir::itos(
			this->admClientes->cantidadDeClientesConectados()));
		respuesta.append(COMMON_DELIMITER);
		respuesta.append(Convertir::itos(
			this->admClientes->cantidadDeCarpetasActivas()));
		respuesta.append(COMMON_DELIMITER);
		int bytesAlmacenados = Recolector::cantidadBytesAlmacenados();
		respuesta.append(Convertir::itos(bytesAlmacenados));

		// Emitimos respuesta
		com->emitir(respuesta);
	}
	else if (instruccion == M_SERVER_USER_LIST_REQUEST) {
		// Se pide a admin cuentas que devuelva la lista de clientes
		Lista<std::string> listaUsuarios;
		admin.obtenerListaUsuarios(listaUsuarios);

		// Se prepara el mensaje
		size_t i, tam = listaUsuarios.tamanio();
		std::string respuesta = S_SERVER_USER_LIST + " ";

		for (i = 0; i < tam - 1; i++)
			respuesta.append(listaUsuarios[i] + COMMON_DELIMITER);
		// Se agrega el ultimo parametro
		respuesta.append(listaUsuarios[i]);

		// Emitimos respuesta
		com->emitir(respuesta);
	}
	else if (instruccion == M_SERVER_MODIFY_USER_REQUEST) {
		// Variables aux
		Lista<std::string> listaArgs;

		// Se obtiene nombre y clave
		Parser::dividirCadena(args, &listaArgs, COMMON_DELIMITER[0]);

		// Se modifica la clave
		admin.modificarCliente(listaArgs[0], listaArgs[1], false);
	}

	else if (instruccion == M_SERVER_NEW_USER_INFO) {
		// Variables aux
		Lista<std::string> listaArgs;

		// Se obtiene nombre y clave
		Parser::dividirCadena(args, &listaArgs, COMMON_DELIMITER[0]);

		// Se agrega cliente al archivo
		admin.agregarCliente(listaArgs[0], listaArgs[1]);
	}
	else if (instruccion == M_SERVER_DELETE_USER) {
		// Se elimina el usuario con el nombre especificado
		admin.eliminarCliente(args);
	}
	else if (instruccion == M_SERVER_LOG_REQUEST) {
		// Q HACEMO?
		std::string respuesta;
		respuesta += S_SERVER_LOG + " ";
		respuesta.append("mensaje de log    ");
		com->emitir(respuesta);
	}
	else if (instruccion == M_SERVER_MODIFY_USER) {
		// se te envia usuario viejo/usuario nuevo/clave
		// buscas por el viejo y modificas la clave siempre..pisas todo
		std::cout<<"info para modificar usuario"<<std::endl;
		std::cout<<mensaje<<std::endl;
	}	
}
