#include "../includes/G-2301-05-P3-ssl.h"
/**
 * Servidor Echo
 * Este servidor se encarga de crear una conexión segura donde se limita a recibir datos de un cliente y devolvérselos inmediatamente
 * Si recibe un exit finaliza
 * @return      0 si todo fue bien, -1 si hubo algún error
 */
int main(int argc, char *argv[]) {
	int num = 512;
	char buf[num];
	int sockfd, newsockfd, portno;
	struct sockaddr_in serv_addr, cli_addr;
	portno = 6667;

	sockfd = openSocket_TCP();
	if (bindSocket_TCP(sockfd, portno, &serv_addr) == -1) {
		close(sockfd);
		exit(1);
	}

	if (listen(sockfd, 50) != 0)//mark sockfd as a socket that will be used to accept incoming connection requests
		return logIntError(1, "error @ main -> listen");

	inicializar_nivel_SSL();
	SSL_CTX* ctx = fijar_contexto_SSL("certs/ca.pem", "certs/servidor.pem", "certs/servidor.pem", NULL);
	if (ctx == NULL) {
		return logIntError(-1, "error @ main -> fijar_contexto_SSL");
	}
	newsockfd = acceptConnection(sockfd, &cli_addr);
	if (newsockfd == -1)
		logIntError(-1, "error @ main -> acceptConnection");
	SSL* ssl = aceptar_canal_seguro_SSL(ctx, newsockfd);
	if (ssl == NULL) {
		return logIntError(-1, "error @ main -> aceptar_canal_seguro_SSL");
	}
	if (evaluar_post_connectar_SSL(ssl) == FALSE)
		return logIntError(-1, "error @ main -> evaluar_post_connectar_SSL");


	do {
		bzero(buf, num);
		if (recibir_datos_SSL(ssl, buf, num) <= 0) {
			printf("el dato recibido es mal\n");
			break;
		}
		if (enviar_datos_SSL(ssl, buf, num) <= 0) {
			printf("el dato recibido es mal\n");
			break;
		}
	} while (strcmp(buf, "exit"));

	cerrar_canal_SSL(ssl, ctx, newsockfd);
	return 0;
}




