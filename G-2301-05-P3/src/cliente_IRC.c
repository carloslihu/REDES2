#include "../includes/G-2301-05-P3-ssl.h"

int main(int argc, char *argv[]) {
    SSL_CTX*ctx;
    SSL*ssl;
    int sockfd, port;
    int num = 512, i = 0;
    char buf[num];
    printf("funciona\n");
    if (argc < 2) {
        port = 6667;
    } else {
    	printf("funciona2\n");
        if (strcmp(argv[1], "--ssldata") == 0) {
            memset(buf, 0, num);
            for (i = 2; strcmp(argv[i], "port"); i++) {
                strncat(buf, argv[i], 20);
                strncat(buf, " ", 20);
            }
        }
        printf("funciona3\n");
        if (strcmp(argv[i], "port") == 0)
            port = atoi(argv[i + 1]);
    }
    printf("funciona\n");
    inicializar_nivel_SSL();
    ctx = fijar_contexto_SSL("certs/ca.pem", "certs/cliente.pem", "certs/cliente.pem", NULL);
    if (ctx == NULL)
        return logIntError(-1, "error @ main -> fijar_contexto_SSL");
    sockfd = openSocket_TCP();
    printf("funciona\n");
    if (connectTo(sockfd, "localhost", port) == -1)
        return logIntError(-1, "Error @ IRCInterface_Connect -> connectTo");
    ssl = conectar_canal_seguro_SSL(ctx, sockfd);
    if (ssl == NULL) {
        return logIntError(-1, "error @ main -> conectar_canal_seguro_SSL");
    }
    if (evaluar_post_connectar_SSL(ssl) == FALSE)
        return logIntError(-1, "error @ main -> evaluar_post_connectar_SSL");
    printf("funciona\n");
    if (enviar_datos_SSL(ssl, buf, num) <= 0) {
        printf("Mecago en todo\n");
    }
    if (recibir_datos_SSL(ssl, buf, num) <= 0) {
        printf("Mecago en todo otra vez\n");
    }
    printf("%s", buf);
    cerrar_canal_SSL(ssl, ctx, sockfd);
    return 0;
}