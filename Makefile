.PHONY: all clean configure build

all: build

clean:
	rm -rf build

configure:
	mkdir -pv build
	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	cp build/compile_commands.json src/

build:
	cd build && make
