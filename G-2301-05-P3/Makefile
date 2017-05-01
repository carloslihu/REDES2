IDIR=includes
ODIR=obj
SDIR=src
LDIR=lib
SLDIR=srclib
CDIR=certs
CC=gcc
CFLAGS= -g -Wall -pthread -rdynamic `pkg-config --cflags gtk+-3.0`
LIBS=-lssl -lcrypto -lircinterface -lircredes -lirctad -lsoundredes -ggdb `pkg-config --libs gtk+-3.0` -L$(LDIR) -lredes2

#CFLAGS2= -g -Wall -pthread
#LIBS2= -lssl -lcrypto -lircinterface -lircredes -lirctad -lsoundredes -ggdb -L$(LDIR) -lredes2

_DEPS =commands.h tools.h socket.h
_DEPS2=userCommands.h userTools.h basicCommandsFromServer.h repliesFromServer.h errorsFromServer.h
_DEPS3=ssl.h

#_OBJ =socket.o tools.o commands.o
_OBJ =commands.o
_OBJ2 =basicCommandsFromServer.o errorsFromServer.o repliesFromServer.o userTools.o userCommands.o
#_OBJ3 = ssl.o

_OBJL = G-2301-05-P1-socket.o G-2301-05-P1-tools.o G-2301-05-P3-ssl.o

DEPS = $(patsubst %,$(IDIR)/G-2301-05-P1-%,$(_DEPS))
DEPS2 = $(patsubst %,$(IDIR)/G-2301-05-P2-%,$(_DEPS2))
DEPS3 = $(patsubst %,$(IDIR)/G-2301-05-P3-%,$(_DEPS3))

OBJ = $(patsubst %,$(ODIR)/G-2301-05-P1-%,$(_OBJ))
OBJ2 = $(patsubst %,$(ODIR)/G-2301-05-P2-%,$(_OBJ2))
#OBJ3 = $(patsubst %,$(ODIR)/G-2301-05-P3-%,$(_OBJ3))

OBJL= $(patsubst %,$(ODIR)/%,$(_OBJL))

all: $(LDIR)/libredes2.a G-2301-05-P1-server 	G-2301-05-P2-xchat2		certificados	servidor_echo	cliente_echo	servidor_IRC

$(ODIR)/%.o: $(SLDIR)/%.c $(DEPS) $(DEPS2) $(DEPS3)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS) $(DEPS2) $(DEPS3)
	$(CC) -c -o $@ $< $(CFLAGS)

$(LDIR)/libredes2.a: $(OBJL)
	ar rcs $@ $^

#EJECUTABLES
G-2301-05-P1-server: $(OBJ) $(ODIR)/G-2301-05-P1-server.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
G-2301-05-P2-xchat2: $(OBJ) $(OBJ2) $(ODIR)/G-2301-05-P2-xchat2.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


servidor_echo: $(OBJ) $(OBJL) $(ODIR)/servidor_echo.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	mv servidor_echo echo/
cliente_echo: $(OBJ) $(OBJL) $(ODIR)/cliente_echo.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	mv cliente_echo echo/

servidor_IRC: $(OBJ) $(OBJL) $(ODIR)/servidor_IRC.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	mv servidor_IRC cliente_servidor/

cliente_IRC: $(OBJ) $(OBJL) $(ODIR)/cliente_IRC.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	mv cliente_IRC cliente_servidor/
#getCommandList: src/G-2301-05-P1-getCommandList.c
#	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
#G-2301-05-P2-userCommands: $(OBJ) $(OBJ2) $(ODIR)/G-2301-05-P2-userCommands.o 
#	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
tar:
	tar -czvf G-2301-05-P2.tar.gz ../G-2301-05-P2/

certificados:
	#CA
	openssl genrsa -out $(CDIR)/rootkey.pem 2048
	openssl req -new -x509 -key $(CDIR)/rootkey.pem -out $(CDIR)/ca.pem -subj "/CN=Redes2 CA"
	cat $(CDIR)/ca.pem $(CDIR)/rootkey.pem > $(CDIR)/root.pem
	
	#cliente
	openssl genrsa -out $(CDIR)/clientkey.pem 2048
	openssl req -new -key $(CDIR)/clientkey.pem -out $(CDIR)/cliente.csr -subj "/CN=G-2301-05-P3-client"
	openssl x509 -req -in $(CDIR)/cliente.csr -CA $(CDIR)/ca.pem -CAkey $(CDIR)/rootkey.pem -out $(CDIR)/cliente.pem -CAcreateserial
	cat $(CDIR)/cliente.pem $(CDIR)/clientkey.pem $(CDIR)/ca.pem > $(CDIR)/client.pem
	
	#server
	openssl genrsa -out $(CDIR)/serverkey.pem 2048
	openssl req -new -key $(CDIR)/serverkey.pem -out $(CDIR)/servidor.csr -subj "/CN=G-2301-05-P3-server"
	openssl x509 -req -in $(CDIR)/servidor.csr -CA $(CDIR)/ca.pem -CAkey $(CDIR)/rootkey.pem -out $(CDIR)/servidor.pem -CAcreateserial
	cat $(CDIR)/servidor.pem $(CDIR)/serverkey.pem $(CDIR)/ca.pem > $(CDIR)/server.pem
	
	rm $(CDIR)/*key.pem
	rm $(CDIR)/*.csr

	mv $(CDIR)/root.pem $(CDIR)/ca.pem
	mv $(CDIR)/server.pem $(CDIR)/servidor.pem
	mv $(CDIR)/client.pem $(CDIR)/cliente.pem

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm -f *.tar.gz
	rm G-2301-05-P1-server G-2301-05-P2-xchat2 echo/servidor_echo echo/cliente_echo cliente_servidor/servidor_IRC
	rm $(CDIR)/* $(LDIR)/*