LD = c99
CC = c99

O = main.o
SDF_LDFLAGS = `sdf.config --libs file`
SDF_CFLAGS = `sdf.config --cflags file`
MATH_LDFLAGS=-lm

main: $O; $(LD) -o $@ $< $(LDFLAGS) $(SDF_LDFLAGS) $(MATH_LDFLAGS)
%.o: %.c; $(CC) $(CFLAGS) $(SDF_CFLAGS) -c -o $@ $<

clean:; rm -f main $O
.PHONY: clean
