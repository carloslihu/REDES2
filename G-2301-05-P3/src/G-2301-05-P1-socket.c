#include "../includes/G-2301-05-P1-socket.h"

/**
 * @brief abre un socket
 *
 * @return el socket o -1 en caso de que hubiera un fallo
 */
int openSocket_TCP() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        return logIntError(-1, "error @ openSocket_TCP");
    return sockfd;
}

/**
 * @brief enlaza un socket con un puerto a la vez que rellena los campos de la estructura sockaddr_in de serv_addr
 *
 * @param sockfd el socket a enlazar
 * @param portno el puerto al que enlazar el socket
 * @param serv_addr la dirección de una estructura sockaddr_in en donde se guardarán los valores relativos a esta conexión
 *
 * @return -1 en caso de error, un numero positivo en otro caso
 */
int bindSocket_TCP(int sockfd, int portno, struct sockaddr_in* serv_addr) {
    int bindReturn;

    memset((char*) serv_addr, 0, sizeof (*serv_addr));
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_addr.s_addr = INADDR_ANY;
    serv_addr->sin_port = htons(portno);
    bindReturn = bind(sockfd, (struct sockaddr *) serv_addr, sizeof (*serv_addr));
    if (bindReturn < 0)
        return logIntError(-1, "Error @ bindSocket_TCP");
    return bindReturn;
}

/**
 * @brief prepara un socket para aceptar una conexión entrante
 *
 * @param sockfd el socket que está escuchando conexiones entrantes
 * @param cli_addr la dirección de una estructura sockaddr_in donde se rellenaran los valores relativos al otro extremo de la conexión
 *
 * @return -1 en caso de error y un nuevo socket (valor mayor que 0) en el que se mantendrá la comunicación con el otro extremo
 */
int acceptConnection(int sockfd, struct sockaddr_in *cli_addr) {

    int newSocket;
    unsigned int clilen = sizeof (*cli_addr);
    newSocket = accept(sockfd, (struct sockaddr *) cli_addr, &clilen);
    if (newSocket < 0){
        perror("errno: ");
        return logIntError(-1, "Error @ acceptConnection");
    }
    return newSocket;
}

/**
 * @brief trata de conectarse a un puerto de un host
 *
 * @param sockfd el socket que intentará realizar la conexión
 * @param hostname el nombre de la maquina a la que se desea conectarse
 * @param portno el puerto al que se desea conectar
 *
 * @return en caso de error, un valor negativo. De lo contrario, un nuevo socket en el que se mantendrá la comunicación con el otro extremo
 */
int connectTo(int sockfd, char* hostname, int portno) {
    int connectReturn;
    struct hostent *server;
    struct sockaddr_in serv_addr;
    server = gethostbyname(hostname);
    if (server == NULL)
        return logIntError(-1, "error @ connectTo->gethostbyname");
    memset((char*) &serv_addr, 0, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char*) &serv_addr.sin_addr.s_addr, (char*) server->h_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    connectReturn = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr));
    if (connectReturn < 0)
        return logIntError(errno, "error @ connectTo -> connect");
    return connectReturn;
}

/**
 * @brief trata de conectarse a un puerto de una IP
 *
 * @param sockfd el socket que intentará realizar la conexión
 * @param IP la dirección IP en notación estándar a la que conectarse
 * @param portno el puerto al que se desea conectar
 *
 * @return en caso de error, un valor negativo. De lo contrario, un nuevo socket en el que se mantendrá la comunicación con el otro extremo
 */
int connectToIP(int sockfd, char* IP, int portno){
    int connectReturn;
    struct sockaddr_in serv_addr;
    memset((char*) &serv_addr, 0, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char*) &serv_addr.sin_addr.s_addr, IP, strlen(IP));
    serv_addr.sin_port = htons(portno);
    connectReturn = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr));
    if (connectReturn < 0)
        return logIntError(errno, "error @ connectTo -> connect");
    return connectReturn;
}
