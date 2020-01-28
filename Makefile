default: run

folder:
	mkdir -p build

build:  folder
	gcc -o build/iso6346 lib/libiso6346.c src/iso6346.c

debug:  folder
	gcc -g -Wall -o build/iso6346_debug lib/libiso6346.c src/iso6346.c

test:   folder
	gcc -g -Wall -o build/test lib/libiso6346.c test/test.c

run: build

