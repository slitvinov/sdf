LD = c99
CC = c99

O = main.o
SDF_LDFLAGS = `sdf.config --libs kernel && sdf.config --libs integration && \
               sdf.config --libs file && sdf.config --libs kernel_conf`
SDF_CFLAGS = `sdf.config --cflags kernel kernel_conf integration file`
MATH_LDFLAGS=-lm

main: $O; $(LD) -o $@ $< $(LDFLAGS) $(SDF_LDFLAGS) $(MATH_LDFLAGS)
%.o: %.c; $(CC) $(CFLAGS) $(SDF_CFLAGS) -c -o $@ $<

clean:; rm -f main $O
.PHONY: clean
