BLDDIR = ./build/
SRCDIR = ./src/

all: $(BLDDIR)rle $(BLDDIR)unrle

$(BLDDIR)rle: $(BLDDIR)rle.o
	gcc -Wall -o $(BLDDIR)rle $(BLDDIR)rle.o

$(BLDDIR)rle.o: $(SRCDIR)rle.c
	gcc -Wall -c $(SRCDIR)rle.c -o $(BLDDIR)rle.o

$(BLDDIR)unrle: $(BLDDIR)unrle.o
	gcc -Wall -o $(BLDDIR)unrle $(BLDDIR)unrle.o

$(BLDDIR)unrle.o: $(SRCDIR)unrle.c
	gcc -Wall -c $(SRCDIR)unrle.c -o $(BLDDIR)unrle.o

clean:
	rm -f $(BLDDIR)*
