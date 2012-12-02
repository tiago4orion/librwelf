CC?=gcc

OBJS=src/*.o

rwelf: librwelf
	$(CC) -otools/rwelf/rwelf -Iinclude/ tools/rwelf/main.c -L. -lrwelf

librwelf:
	$(CC) -fPIC -g -c -Wall -Iinclude/ -osrc/elf.o src/elf.c
	$(CC) -fPIC -g -c -Wall -Iinclude/ -osrc/header.o src/header.c
	$(CC) -shared -Wl,-soname,librwelf.so.0 -olibrwelf.so.0.1.0 $(OBJS)
	ln -sf librwelf.so.0.1.0 librwelf.so.0
	ln -sf librwelf.so.0.1.0 librwelf.so

clean:
	rm -rf tools/rwelf/rwelf librwelf.so* src/*.o
	