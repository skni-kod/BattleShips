.PHONY: all configure build

all: build

configure:
	mkdir -pv build
	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	cp build/compile_commands.json src/

build:
	cd build && make && ./battleships
