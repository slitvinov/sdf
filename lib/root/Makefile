PREFIX = $(HOME)
S = ../..
I = inc
B = .
O = $B/main.o
L = $B/libsdf_root.a
H = inc/sdf_root.h

SDF_CFLAGS = `sdf.config --cflags gsl`

all: install

include $S/make/lib.mk

$B/main.o: src/main.c; $(CC) $(CFLAGS) -Iinc -Isrc $(SDF_CFLAGS) -c -o $@ $<
$B/main.o: inc/sdf_root.h
