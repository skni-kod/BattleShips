.PHONY: all clean configure build

all: build

clean:
	rm -rf build

configure:
	mkdir -pv build
	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	cp build/compile_commands.json src/

docs:
	mkdir -pv build
	cd build && cmake -DENABLE_DOXYGEN=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	cp build/compile_commands.json src/
	doxygen build/Doxyfile.doxygen-docs
	cd build/latex && make


build:
	cd build && make
