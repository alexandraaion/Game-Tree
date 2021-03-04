build: minimax

minimax: tema2.o tree.o tree.h
	gcc -o minimax tema2.o tree.o

tema2.o: tema2.c tree.h
	gcc -c -g tema2.c

tree.o: tree.c tree.h
	gcc -c -g tree.c

clean:
	rm *.o minimax
