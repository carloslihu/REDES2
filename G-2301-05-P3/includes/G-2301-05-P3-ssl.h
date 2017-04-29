/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ssl.h
 * Author: carlos
 *
 * Created on 11 de abril de 2017, 17:27
 */

#ifndef SSL_H
#define SSL_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "G-2301-05-P2-userCommands.h"

void inicializar_nivel_SSL();
SSL_CTX* fijar_contexto_SSL(char* CAfile,char* SCfile, char* Pfile, char*CApath);
//SSL_CTX* fijar_contexto_SSL(char* CAfile,char*CApath);
SSL* conectar_canal_seguro_SSL(SSL_CTX* ctx, int sockfd);
SSL* aceptar_canal_seguro_SSL(SSL_CTX* ctx, int sockfd);
boolean evaluar_post_connectar_SSL(SSL*ssl);
int enviar_datos_SSL(SSL* ssl, void* buf, int num);
int recibir_datos_SSL(SSL* ssl, void* buf, int num);
int cerrar_canal_SSL(SSL* ssl, SSL_CTX* ctx, int sockfd);

#endif /* SSL_H */

