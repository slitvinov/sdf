LD = c99
CC = c99

O = main.o
SDF_LDFLAGS = `dq.config --libs memory`
SDF_CFLAGS = `dq.config --cflags memory`

main: $O; $(LD) -o $@ $< $(LDFLAGS) $(SDF_LDFLAGS)
%.o: %.c; $(CC) $(CFLAGS) $(SDF_CFLAGS) -c -o $@ $<

clean:; rm -f main $O
.PHONY: clean
