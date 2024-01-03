INCLUDE:=-I/usr/local/include/kubernetes/
LIBS:=-lyaml -lwebsockets -lkubernetes -L/usr/local/lib
CFLAGS:=-g
BIN:=patch-deployment
LDFLAGS:=-Wl,-rpath,/usr/local/lib

.PHONY : all clean test memcheck
all:
	gcc main.c $(CFLAGS) $(INCLUDE) $(LIBS) $(LDFLAGS) -o $(BIN)

test:
	./$(BIN)

memcheck:
	valgrind --tool=memcheck --leak-check=full ./$(BIN)

clean:
	rm ./$(BIN)
