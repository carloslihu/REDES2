#include "../includes/G-2301-05-P3-ssl.h"
/**
 * Cliente Echo
 * Se encarga de crear una conexión segura donde se manda al servidor lo escrito por terminal y se dedica a imprimir lo que recibe
 * Si escribes exit acaba su ejecución
 * @return      0 si es correcto, -1 si no
 */
int main(int argc, char *argv[]) {
	SSL_CTX*ctx;
	SSL*ssl;
	int sockfd;
	int num = 512;
	char buf[num];

	inicializar_nivel_SSL();
	ctx = fijar_contexto_SSL("certs/ca.pem", "certs/cliente.pem", "certs/cliente.pem", NULL);
	if (ctx == NULL)
		return logIntError(-1, "error @ main -> fijar_contexto_SSL");
	sockfd = openSocket_TCP();
	if (connectTo(sockfd, "localhost", 6667) == -1)
		return logIntError(-1, "Error @ IRCInterface_Connect -> connectTo");
	ssl = conectar_canal_seguro_SSL(ctx, sockfd);
	if (ssl == NULL) {
		return logIntError(-1, "error @ main -> conectar_canal_seguro_SSL");
	}
	if (evaluar_post_connectar_SSL(ssl) == FALSE)
		return logIntError(-1, "error @ main -> evaluar_post_connectar_SSL");
	do {
		scanf("%s[^\n]\n", buf);
		if (enviar_datos_SSL(ssl, buf, num) <= 0) {
			break;
		}
		bzero(buf, num);
		if (recibir_datos_SSL(ssl, buf, num) <= 0) {
			break;
		}
		printf("%s\n", buf);
	} while (strcmp(buf, "exit"));

	cerrar_canal_SSL(ssl, ctx, sockfd);
	return 0;
}
