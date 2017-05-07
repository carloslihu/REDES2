#include "../includes/G-2301-05-P3-ssl.h"

int main(int argc, char *argv[]) {
    SSL_CTX*ctx;
    SSL*ssl;
    int sockfd, port;
    int num = 512, i = 0;
    char buf[num];
    if (argc < 2) {
        port = 6669;
    } else {
        /*FILE*f = fopen("/home/carlos/Escritorio/Redes/prueba.txt", "w+");
        for (i = 0; i < argc; i++)
            fprintf(f, "%s\n", argv[i]);
        fclose(f);*/
        if (strcmp(argv[1], "--ssldata") == 0) {
            memset(buf, 0, num);
            strncat(buf, argv[2], 20);
            for (i = 3; strcmp(argv[i], "port") && i<argc; i++) {
                strncat(buf, " ", 20);
                strncat(buf, argv[i], 20);
                //printf("%s ", argv[i]);

            }
        }
        //if (strcmp(argv[i], "port") == 0)
        //    port = atoi(argv[i + 1]);
        //else{
        	port=6669;
        //}
        //strcpy(buf,"NICK yoda\r\nUSER yoda 0 * :miservidor.com\r\n");

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
        printf("Mecago en todo\n");
    }
    if (recibir_datos_SSL(ssl, buf, num) <= 0) {
        printf("Mecago en todo otra vez\n");
    }
    printf("%s", buf);
    cerrar_canal_SSL(ssl, ctx, sockfd);
    return 0;
}