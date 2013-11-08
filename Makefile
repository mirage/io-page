PHONY: all unix xen

build:
	./opam_build false

%-build:
	OS=$* ./build

install:
	./opam_build true

%-install:
	OS=$* ./build true

clean:
	rm -rf _build
