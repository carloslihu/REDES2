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

_OBJ =commands.o

_OBJL = G-2301-05-P1-socket.o G-2301-05-P1-tools.o

DEPS = $(patsubst %,$(IDIR)/G-2301-05-P1-%,$(_DEPS))

OBJ = $(patsubst %,$(ODIR)/G-2301-05-P1-%,$(_OBJ))

OBJL= $(patsubst %,$(ODIR)/%,$(_OBJL))

all: $(LDIR)/libredes2.a G-2301-05-P1-server
$(ODIR)/%.o: $(SLDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(LDIR)/libredes2.a: $(OBJL)
	ar rcs $@ $^

#EJECUTABLES
#P1
G-2301-05-P1-server: $(OBJ) $(ODIR)/G-2301-05-P1-server.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#P1
tar:
	tar -czvf G-2301-05-P1.tar.gz ../G-2301-05-P1/

.PHONY: clean
clean:
	-rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	-rm -f *.tar.gz
	-rm $(LDIR)/*
	#P1
	-rm G-2301-05-P1-server 

