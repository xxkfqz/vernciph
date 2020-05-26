CC ?= cc
CFLAGS ?= -O3 -march=native
PROGRAM ?= vernciph
STRIP ?= strip
PREFIX ?= /usr/local

default: vernciph

vernciph: vernciph.c
	$(CC) vernciph.c $(CFLAGS) -o $(PROGRAM)
	$(STRIP) -s $(PROGRAM)

install: vernciph
	mkdir -p ${PREFIX}/bin
	cp -f $(PROGRAM) ${PREFIX}/bin/${PROGRAM}
	chmod 755 ${PREFIX}/bin/${PROGRAM}

clean:
	rm -f $(PROGRAM)

.PHONY: vernciph
