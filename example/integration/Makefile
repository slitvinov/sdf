LD = c99
CC = c99

O = main.o
SDF_LDFLAGS = `sdf.config --libs integration`
SDF_CFLAGS = `sdf.config --cflags integration`

main: $O; $(LD) -o $@ $< $(LDFLAGS) $(SDF_LDFLAGS)
%.o: %.c; $(CC) $(CFLAGS) $(SDF_CFLAGS) -c -o $@ $<

clean:; rm -f main $O
.PHONY: clean
