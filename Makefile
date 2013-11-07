PHONY: all

all:
	./opam_build false

unix:
	OS=unix ./build

xen:
	OS=unix ./build

install:
	./opam_build true

clean:
	rm -rf _build
