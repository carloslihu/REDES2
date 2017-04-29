#include "../includes/G-2301-05-P3-ssl.h"

int main(int argc, char *argv[]) {
    SSL_CTX*ctx;
    SSL*ssl;
    int sockfd, port;
    int num = 512;
    char buf[num];
    if (argc < 2) {
        port = 6667;
        strcpy(buf,"NICK yoda");
    } else {
        if (strcmp(argv[1], "--ssldata") == 0) {
            strcpy(buf, argv[2]);
        }
        if (strcmp(argv[3], "port") == 0)
            port = atoi(argv[4]);
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
    /*do {
        scanf("%s[^\n]\n",buf);
        if (enviar_datos_SSL(ssl, buf, num) <= 0) {
            break;
        }
        bzero(buf, num);
        if (recibir_datos_SSL(ssl,buf,num) <= 0) {
            break;
        }
        printf("%s\n",buf);
    }while (strcmp(buf, "exit"));*/
    if (enviar_datos_SSL(ssl, buf, num) <= 0) {

    }
    if()
    if (recibir_datos_SSL(ssl, buf, num) <= 0) {
    }
    printf("%s",buf);
    cerrar_canal_SSL(ssl, ctx, sockfd);
    return 0;
}