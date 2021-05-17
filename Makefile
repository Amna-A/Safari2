CC := g++
CCOPTS=-Wall -Wextra -O3 -std=c++17

Safari_DEP_OBJ := Safari.o State.o Search.o
Header:=State.h Search.h

all: safari

%.o: %.cpp $(Header)
	$(CC) ${CCOPTS}  -c -o $@ $<

safari: $(Safari_DEP_OBJ)
	${CC} ${CCOPTS} -o safari $^

clean:
	rm -f Safari *.o