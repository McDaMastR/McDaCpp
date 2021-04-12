CC = clang
WFLAGS = -Wall -Wextra -Wpedantic
DFLAGS = -g -Og -DDEBUG $(WFLAGS)

OBJ_FILES = bin/int/main.o bin/int/nodeFuncs.o bin/int/hashTable.o

all: bin/LinkedLists

bin/int/%.o: src/%.c src/include/%.h
	$(CC) $(DFLAGS) -c -o $@ $<

bin/int/main.o: src/main.c
	$(CC) $(DFLAGS) -c -o $@ $^

bin/LinkedLists: $(OBJ_FILES)
	$(CC) $(DFLAGS) -o $@ $^

clean:
	rm bin/int/*.o bin/LinkedLists 
	rm -r bin/LinkedLists.dSYM
