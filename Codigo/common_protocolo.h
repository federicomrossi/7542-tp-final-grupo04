//  
//  common_protocolo.h
//  
//  Cabecera con constantes que especifican el protocolo de mensajes a
//  ser utilizados por el cliente y el servidor.
//  


#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#include <string>




/* ****************************************************************************
 * PROTOCOLO DE INSTRUCCIONES
 * ***************************************************************************/


// Constantes para los identificadores de instrucciones enviadas por el
// cliente
// FORMATO DE LAS CONSTANTES: C_[instruccion]
const std::string C_LOGIN_REQUEST = "LOGIN-REQUEST";
const std::string C_GET_FILES_LIST = "GET-FILES-LIST";
const std::string C_FILE_REQUEST = "FILE-REQUEST";

// Constantes para los identificadores de instrucciones enviadas por el
// servidor
// FORMATO DE LAS CONSTANTES: S_[instruccion]
const std::string S_LOGIN_OK = "LOGIN-OK";
const std::string S_LOGIN_FAIL = "LOGIN-FAIL";
const std::string S_FILES_LIST = "FILES-LIST";
const std::string S_NOTIFY_CHANGE = "FILE-CHANGED";
const std::string S_NOTIFY_NEW = "NEW-FILE";
const std::string S_NO_SUCH_FILE = "NO-SUCH-FILE";

// Constantes para los identificadores de instrucciones comunes al servidor
// y al cliente
// FORMATO DE LAS CONSTANTES: COMMON_[instruccion]
const std::string COMMON_MSG_IS_NOTIFICATION = "n";
const std::string COMMON_MSG_IS_FILE = "f";
const std::string COMMON_SEND_FILE = "SEND-FILE";
const std::string COMMON_MODIFY_FILE = "MODIFY-FILE";
const std::string COMMON_DELETE_FILE = "DELETE-FILE";
const char COMMON_FILE_PARAMETER_DELIMITER = ',';

// Constante para caracter de fin de instrucción
const char FIN_MENSAJE = '\n';

// Constante para hacer referencia al archivo entero
const std::string WHOLE_FILE = "FFFF";


#endif
