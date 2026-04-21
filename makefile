CFLAGS = -g -O0 -Wall

main: main.o pag_inicio.o pages_methods.o lists.o
	gcc $(CFLAGS) obj/main.o obj/lists.o obj/pag_inicio.o obj/pages_methods.o -o main

main.o: main.c
	gcc $(CFLAGS) -c main.c -o obj/main.o

pag_inicio.o: pag_inicio.c
	gcc $(CFLAGS) -c pag_inicio.c -o obj/pag_inicio.o

pages_methods.o: pages_methods.c
	gcc $(CFLAGS) -c pages_methods.c -o obj/pages_methods.o

lists.o: lists.c
	gcc $(CFLAGS) -c lists.c -o obj/lists.o

clean:
	del main.exe
	del obj\main.o
	del obj\pag_inicio.o
	del obj\pages_methods.o
	del obj\lists.o
