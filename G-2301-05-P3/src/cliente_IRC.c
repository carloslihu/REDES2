#include "../includes/G-2301-05-P3-ssl.h"

int main(int argc, char *argv[]) {
    SSL_CTX*ctx;
    SSL*ssl;
    int sockfd, port;
    int num = 512, i = 0;
    char buf[num];
    if (argc < 2) {
        port = 6669;
        strcpy(buf, "NICK yoda\r\n");
    } else {
        if (argv[1][0] == '-') {
            memset(buf, 0, num);
            for (i = 2; /*strcmp(argv[i], "port") &&*/ i < argc; i++) {
                strcat(buf, argv[i]);
                strcat(buf, " ");

            }
            strcat(buf, "\r\n");
        }
        port = 6669;

    }
    inicializar_nivel_SSL();
    ctx = fijar_contexto_SSL("certs/ca.pem", "certs/cliente.pem", "certs/cliente.pem", NULL);
    if (ctx == NULL)
        return logIntError(-1, "error @ main -> fijar_contexto_SSL");
    sockfd = openSocket_TCP();
    if (connectTo(sockfd, "localhost", port) == -1)
        return logIntError(-1, "Error @ IRCInterface_Connect -> connectTo");

    ssl = conectar_canal_seguro_SSL(ctx, sockfd);
    if (ssl == NULL) {
        return logIntError(-1, "error @ main -> conectar_canal_seguro_SSL");
    }

    if (evaluar_post_connectar_SSL(ssl) == FALSE)
        return logIntError(-1, "error @ main -> evaluar_post_connectar_SSL");
    if (enviar_datos_SSL(ssl, buf, strlen(buf)) <= 0) {
        return logIntError(-1, "error @ main -> enviar_datos_SSL");
    }
    cerrar_canal_SSL(ssl, ctx, sockfd);
    return 0;
}