//
//  client_inspector.h
//  CLASE INSPECTOR
//  


#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <string>
#include "common_thread.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Inspector : public Thread {
private:

	unsigned int intervalo;				// Intervalo de inspección en segundos


	// Bloquea actividades hasta que haya transcurrido el intervalo de polling
	void alarmaDeInspeccion();

public:

	// Constructor
	// PRE: 'intervalo' es el intervalo de inspección en segundos.
	Inspector(unsigned int intervalo);

	// Destructor
	~Inspector();

	// Inicia el ciclo de inspecciones
	void iniciar();

	// Detiene el ciclo de inspecciones
	void detener();

	// Setea los segundos de intervalo entre sucesivas inspecciones.
	// PRE: 'segundos' es la cantidad de segundos entre inspecciones.
	void establecerIntervaloDeInspeccion(unsigned int segundos);

	// CONSIDERAR REMOVER ESTE METODO
	void forzarInspeccion();

	// Define tareas a ejecutar en el hilo.
	// Realiza una inspección cada un intervalo predeterminado.
	virtual void run();
};

#endif
