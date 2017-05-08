IDIR=includes
ODIR=obj
SDIR=src
LDIR=lib
SLDIR=srclib
CDIR=certs
CC=gcc
EDIR=echo
SSLDIR=cliente_servidor
CFLAGS= -g -Wall -pthread -rdynamic `pkg-config --cflags gtk+-3.0`
LIBS=-L$(LDIR) -lredes2 -lssl -lcrypto -lircinterface -lircredes -lirctad -lsoundredes -ggdb `pkg-config --libs gtk+-3.0` -lpulse -lpulse-simple

_DEPS =commands.h tools.h socket.h
_DEPS2=userCommands.h userTools.h basicCommandsFromServer.h repliesFromServer.h audio.h


_OBJ =commands.o
_OBJ2 =basicCommandsFromServer.o audio.o repliesFromServer.o userTools.o userCommands.o

_OBJL = G-2301-05-P1-socket.o G-2301-05-P1-tools.o

DEPS = $(patsubst %,$(IDIR)/G-2301-05-P1-%,$(_DEPS))
DEPS2 = $(patsubst %,$(IDIR)/G-2301-05-P2-%,$(_DEPS2))

OBJ = $(patsubst %,$(ODIR)/G-2301-05-P1-%,$(_OBJ))
OBJ2 = $(patsubst %,$(ODIR)/G-2301-05-P2-%,$(_OBJ2))

OBJL= $(patsubst %,$(ODIR)/%,$(_OBJL))

all: $(LDIR)/libredes2.a 	G-2301-05-P2-xchat2
$(ODIR)/%.o: $(SLDIR)/%.c $(DEPS) $(DEPS2)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS) $(DEPS2)
	$(CC) -c -o $@ $< $(CFLAGS)

$(LDIR)/libredes2.a: $(OBJL)
	ar rcs $@ $^

#EJECUTABLES
#P2
G-2301-05-P2-xchat2: $(OBJ) $(OBJ2) $(ODIR)/G-2301-05-P2-xchat2.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#P2
tar:
	tar -czvf G-2301-05-P2.tar.gz ../G-2301-05-P2/

.PHONY: clean
clean:
	-rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	-rm -f *.tar.gz
	-rm $(LDIR)/*
	#P2
	-rm G-2301-05-P2-xchat2 
