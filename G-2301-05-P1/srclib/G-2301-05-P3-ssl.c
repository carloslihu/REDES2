#include "../includes/G-2301-05-P3-ssl.h"
#include <err.h>

/**
 * Esta función se encargará de realizar todas las llamadas necesarias para que la apli-
cación pueda usar la capa segura SSL.
 */
void inicializar_nivel_SSL() {
    SSL_load_error_strings();
    SSL_library_init();
}

/**
 * Esta función se encargará de inicializar correctamente el contexto que será utilizado para
la creación de canales seguros mediante SSL. Deberá recibir información sobre las rutas a los certificados y
claves con los que vaya a trabajar la aplicación.
 * @param  CAfile certificado de la CA
 * @param  SCfile certificado del servidor/cliente
 * @param  Pfile  clave privada del servidor/cliente
 * @param  CApath ruta del certificado de la CA
 * @return        contexto de la conexión segura
 */
SSL_CTX* fijar_contexto_SSL(char* CAfile, char* SCfile, char* Pfile, char*CApath) {
    SSL_CTX* ctx;
    ctx = SSL_CTX_new(SSLv23_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    if (SSL_CTX_load_verify_locations(ctx, CAfile, CApath) == 0) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    if (SSL_CTX_set_default_verify_paths(ctx) == 0) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    if (SSL_CTX_use_certificate_chain_file(ctx, SCfile) <= 0) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, Pfile, SSL_FILETYPE_PEM) != 1) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
    return ctx;
}

/**
 * Dado un contexto SSL y un descriptor de socket esta función se encargará de
obtener un canal seguro SSL iniciando el proceso de handshake con el otro extremo.
 * @param  ctx    contexto de la conexión segura
 * @param  sockfd socket de la conexión segura
 * @return        estructura SSL de la conexión segura
 */
SSL* conectar_canal_seguro_SSL(SSL_CTX* ctx, int sockfd) {
    SSL*ssl;
    int ret;
    ssl = SSL_new(ctx);
    if (SSL_set_fd(ssl, sockfd) == 0) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    //caso cliente
    if ((ret = SSL_connect(ssl)) != 1) {
        ERR_print_errors_fp(stdout);
        err(SSL_get_error(ssl, ret), NULL);
        return NULL;
    }
    return ssl;

}

/**
 * Dado un contexto SSL y un descriptor de socket esta función se encargará de
bloquear la aplicación, que se quedará esperando hasta recibir un handshake por parte del cliente.
 * @param  ctx    contexto de la conexión segura
 * @param  sockfd socket de la conexión segura
 * @return        estructura SSL de la conexión segura
 */
SSL* aceptar_canal_seguro_SSL(SSL_CTX* ctx, int sockfd) {
    SSL*ssl;
    int ret;
    ssl = SSL_new(ctx);
    if (SSL_set_fd(ssl, sockfd) == 0) {
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    //caso servidor
    if ((ret = SSL_accept(ssl)) != 1) {
        ERR_print_errors_fp(stdout);
        err(SSL_get_error(ssl, ret), NULL);
        return NULL;
    }
    return ssl;

}

/**
 * Esta función comprobará una vez realizado el handshake que el canal de co-
municación se puede considerar seguro.
 * @param  ssl estructura SSL de la conexión segura
 * @return     true si es seguro, false si no
 */
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

/**
 * Esta función será el equivalente a la función de envío de mensajes que se realizó en la
práctica 1, pero será utilizada para enviar datos a través del canal seguro. Es importante que sea genérica y
pueda ser utilizada independientemente de los datos que se vayan a enviar.
 * @param  ssl estructura SSL de la conexión segura
 * @param  buf datos a enviar
 * @param  num tamaño de los datos
 * @return     numero >0 si ha ido bien <=0 si no
 */
int enviar_datos_SSL(SSL* ssl, void* buf, int num) {
    return SSL_write(ssl, buf, num);
}

/**
 * Esta función será el equivalente a la función de lectura de mensajes que se realizó en la
práctica 1, pero será utilizada para enviar datos a través del canal seguro. Es importante que sea genérica y
pueda ser utilizada independientemente de los datos que se vayan a recibir.
 * @param  ssl estructura SSL de la conexión segura
 * @param  buf datos a recibir
 * @param  num tamaño de los datos
 * @return     numero >0 si ha ido bien <=0 si no
 */
int recibir_datos_SSL(SSL* ssl, void* buf, int num) {
    return SSL_read(ssl, buf, num);

}

/**
 * Esta función liberará todos los recursos y cerrará el canal de comunicación seguro creado
previamente.
 * @param  ssl    estructura SSL de la conexión segura
 * @param  ctx    contexto de la conexión segura
 * @param  sockfd socket de la conexión segura
 * @return        1 si ha ido bien, -1 si ha habido un fallo
 */
int cerrar_canal_SSL(SSL* ssl, SSL_CTX* ctx, int sockfd) {
    if (ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }
    if (ctx)
        SSL_CTX_free(ctx);
    return 1;
}