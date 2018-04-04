PREFIX = $(HOME)
B = $(PREFIX)/bin

S = ../..
D = prefix
P = sdf
M = main util

include $S/make/tool.mk
main: main.in
	awk -v p="$(PREFIX)" '{gsub(/%PREFIX%/, p); print}' main.in > main
	chmod +x main

clean:; rm -f main
.PHONY: clean

