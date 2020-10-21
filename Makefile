.PHONY: all configure build

all: build

configure:
	mkdir -pv build
	cd build && cmake ..

build:
	cd build && make && ./battleships
