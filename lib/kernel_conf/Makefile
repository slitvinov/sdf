PREFIX = $(HOME)
S = ../..
I = inc
B = .
O = $B/main.o
L = $B/libsdf_kernel_conf.a
H = inc/sdf_kernel_conf.h

SDF_CFLAGS = `sdf.config --cflags memory integration kernel root`

all: install

include $S/make/lib.mk

$B/main.o: src/main.c; $(CC) $(CFLAGS) -Iinc -Isrc $(SDF_CFLAGS) -c -o $@ $<
$B/main.o: inc/sdf_kernel_conf.h
