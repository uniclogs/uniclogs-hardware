IDIR =./
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=build
LDIR =../lib

LIBS=-lm

_DEPS = pwrBoard.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = pwrBoard.o pwrMain.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

pwrcode: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o pwrcode
