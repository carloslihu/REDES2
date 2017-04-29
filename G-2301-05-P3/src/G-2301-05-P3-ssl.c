#include "../includes/G-2301-05-P3-ssl.h"
#include <err.h>
void inicializar_nivel_SSL() {
    SSL_load_error_strings();
    //en caso de error, usar void ERR_print_errors_fp(FILE *fp);
    SSL_library_init();
}

SSL_CTX* fijar_contexto_SSL(char* CAfile, char* SCfile, char* Pfile, char*CApath) {
    SSL_CTX* ctx;
    ctx = SSL_CTX_new(SSLv23_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    /*Es importante resaltar que esta función solo ha de llamarse en caso de 
    querer añadir entidades CA propias fuera de las
    ya preestablecidas*/
    if (SSL_CTX_load_verify_locations(ctx, CAfile, CApath) == 0) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    if (SSL_CTX_set_default_verify_paths(ctx) == 0) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    //???
    if (SSL_CTX_use_certificate_chain_file(ctx, SCfile) <= 0) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    //???
    if (SSL_CTX_use_PrivateKey_file(ctx, Pfile, SSL_FILETYPE_PEM) != 1) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    /*Este modo será el mismo tanto para clientes como para servidores. Habrá que garantizar que se
verifica la autenticidad del otro extremo del canal de comunicación y que se produce un fallo si el otro extremo
no envía el correspondiente certificado. En nuestro caso no utilizaremos función de callback.*/
    //???
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
    return ctx;
}

SSL* conectar_canal_seguro_SSL(SSL_CTX* ctx, int sockfd) {
    SSL*ssl;
    int ret;
    ssl = SSL_new(ctx);
    if (SSL_set_fd(ssl, sockfd) == 0) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    //caso cliente
    if ((ret=SSL_connect(ssl)) != 1) {
        ERR_print_errors_fp(stdout);
        err(SSL_get_error(ssl,ret),NULL);
        return NULL;
    }
    return ssl;

}

SSL* aceptar_canal_seguro_SSL(SSL_CTX* ctx, int sockfd) {
    SSL*ssl;
    int ret;
    ssl = SSL_new(ctx);
    if (SSL_set_fd(ssl, sockfd) == 0) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    //caso servidor
    if ((ret=SSL_accept(ssl)) != 1) {
        ERR_print_errors_fp(stdout);
        err(SSL_get_error(ssl,ret),NULL);
        return NULL;
    }
    return ssl;

}

boolean evaluar_post_connectar_SSL(SSL*ssl) {
    X509* cert = SSL_get_peer_certificate(ssl);
    if (cert == NULL) {
        ERR_print_errors_fp(stdout);
        return FALSE;
    }
    free(cert);
    if (SSL_get_verify_result(ssl) != X509_V_OK) {
        ERR_print_errors_fp(stdout);
        return FALSE;
    }
    return TRUE;
}

int enviar_datos_SSL(SSL* ssl, void* buf, int num) {
    return SSL_write(ssl, buf, num);
}

int recibir_datos_SSL(SSL* ssl, void* buf, int num) {
    return SSL_read(ssl, buf, num);;
}

int cerrar_canal_SSL(SSL* ssl, SSL_CTX* ctx, int sockfd) {
    if (SSL_shutdown(ssl) != 1) {
        ERR_print_errors_fp(stdout);
        return -1;
    }
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    if (close(sockfd) == -1)
        return -1;

    return 1;
}