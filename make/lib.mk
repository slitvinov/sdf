$L: $O; ar rv $@ $O; ranlib $@
clean:; rm -f $O $L
install: $L $H
	mkdir -p $(PREFIX)/lib $(PREFIX)/include
	cp $L $(PREFIX)/lib
	cp $H $(PREFIX)/include
.PHONY: clean install
