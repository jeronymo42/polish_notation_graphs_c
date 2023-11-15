FLAGS=-Wall -Werror -Wextra -fsanitize=address -fsanitize=leak
OUT=../build/graph.out

all: graph clean

graph: graph.c expression_process.o structs.o
	gcc ${FLAGS} -o ${OUT} graph.c expression_process.o structs.o -lm

expression_process.o: expression_process.h expression_process.c
	gcc ${FLAGS} -c expression_process.h expression_process.c

structs.o: structs.h structs.c
	gcc ${FLAGS} -c structs.h structs.c

clean:
	rm -rf *.o
	rm -rf *.gch