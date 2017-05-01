#ifndef _SOCKET_H
#define _SOCKET_H

#include "G-2301-05-P1-tools.h"

#define SERVERINFO "servidor de Mambru, DoReMi DoReFa"
#define SERVERNAME "Mambru@war"

/**
 * @brief abre un socket
 *
 * @return el socket o -1 en caso de que hubiera un fallo
 */
int openSocket_TCP();

/**
 * @brief enlaza un socket con un puerto a la vez que rellena los campos de la estructura sockaddr_in de serv_addr
 *
 * @param sockfd el socket a enlazar
 * @param portno el puerto al que enlazar el socket
 * @param serv_addr la dirección de una estructura sockaddr_in en donde se guardarán los valores relativos a esta conexión
 *
 * @return -1 en caso de error, un numero positivo en otro caso
 */
int bindSocket_TCP(int sockfd, int portno, struct sockaddr_in* serv_addr);

/**
 * @brief prepara un socket para aceptar una conexión entrante
 *
 * @param sockfd el socket que está escuchando conexiones entrantes
 * @param cli_addr la dirección de una estructura sockaddr_in donde se rellenaran los valores relativos al otro extremo de la conexión
 *
 * @return -1 en caso de error y un nuevo socket (valor mayor que 0) en el que se mantendrá la comunicación con el otro extremo
 */
int acceptConnection(int sockfd, struct sockaddr_in* client_addr);

/**
 * @brief trata de conectarse a un puerto de un host
 *
 * @param sockfd el socket que intentará realizar la conexión
 * @param hostname el nombre de la maquina a la que se desea conectarse
 * @param portno el puerto al que se desea conectar
 *
 * @return en caso de error, un valor negativo. De lo contrario, un nuevo socket en el que se mantendrá la comunicación con el otro extremo
 */
int connectTo(int sockfd, char* hostname, int portno);

/**
 * @brief trata de conectarse a un puerto de una IP
 *
 * @param sockfd el socket que intentará realizar la conexión
 * @param IP la dirección IP en notación estándar a la que conectarse
 * @param portno el puerto al que se desea conectar
 *
 * @return en caso de error, un valor negativo. De lo contrario, un nuevo socket en el que se mantendrá la comunicación con el otro extremo
 */
int connectToIP(int sockfd, char* IP, int portno);
#endif