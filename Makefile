BLDDIR = ./build/
SRCDIR = ./src/

all: $(BLDDIR)rle

$(BLDDIR)rle: $(BLDDIR)main.o
	gcc -Wall -o $(BLDDIR)rle $(BLDDIR)main.o

$(BLDDIR)main.o: $(SRCDIR)main.c
	gcc -Wall -c $(SRCDIR)main.c -o $(BLDDIR)main.o

clean:
	rm -f $(BLDDIR)*
