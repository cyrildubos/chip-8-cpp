compile: src/main.cc
	g++ src/main.cc -o CHIP-8 src/chip_8.cc
	
run: compile
	./CHIP-8