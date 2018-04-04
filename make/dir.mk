PREFIX = $(HOME)

install:
	@install0 () ( cd "$$d" && make install PREFIX=$(PREFIX)); \
	for d in $D; \
	do install0; \
	echo dir "'$$d'"; \
	done

example:
	@install0 () ( cd "$$d" && make PREFIX=$(PREFIX)); \
	for d in $E; \
	do install0; \
	echo dir "'$$d'"; \
	done


.PHONY: install example
