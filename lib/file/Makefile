PREFIX = $(HOME)
S = ../..
I = inc
B = .
O = $B/main.o
L = $B/libsdf_file.a
H = inc/sdf_file.h

SDF_CFLAGS = `sdf.config --cflags memory`

all: install

include $S/make/lib.mk

$B/main.o: src/main.c; $(CC) $(CFLAGS) -Iinc -Isrc $(SDF_CFLAGS) -c -o $@ $<
$B/main.o: inc/sdf_file.h
