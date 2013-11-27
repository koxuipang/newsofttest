test : test.o io.o
	@gcc test.o io.o -o test -lpthread
test.o : test.c io.h
	@gcc -c test.c -o test.o
io.o : io.c io.h pinmap.h
	@gcc -c io.c -o io.o
clean :
	@rm *.o test
install :
	@chmod 777 itead.h io.h pinmap.h regmap.h
	@cp itead.h io.h pinmap.h regmap.h /usr/local/include
	@cp itead.h io.h pinmap.h regmap.h /usr/include
	@gcc -shared -fpic -o libiteadIO.so io.c -lpthread
	@sudo cp libiteadIO.so /usr/local/lib
	@sudo cp libiteadIO.so /usr/lib
	
