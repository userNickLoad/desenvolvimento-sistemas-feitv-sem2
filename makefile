CFLAGS = -g -O0 -Wall

all: main.o pag_inicio.o pages_methods.o lists.o pag_login.o pag_signup.o data_utils.o user.o
	gcc $(CFLAGS) obj/main.o obj/lists.o obj/pag_inicio.o obj/pages_methods.o obj/pag_login.o obj/pag_signup.o obj/user.o obj/data_utils.o -o main

main.o: main.c
	gcc $(CFLAGS) -c main.c -o obj/main.o


pag_inicio.o: pags/pag_inicio.c
	gcc $(CFLAGS) -c pags/pag_inicio.c -o obj/pag_inicio.o

pag_login.o: pags/pag_login.c
	gcc $(CFLAGS) -c pags/pag_login.c -o obj/pag_login.o

pag_signup.o: pags/pag_signup.c
	gcc $(CFLAGS) -c pags/pag_signup.c -o obj/pag_signup.o

pages_methods.o: pages_methods.c
	gcc $(CFLAGS) -c pages_methods.c -o obj/pages_methods.o

user.o: data/source/user.c
	gcc $(CFLAGS) -c data/source/user.c -o obj/user.o

data_utils.o: data/source/utils.c
	gcc $(CFLAGS) -c data/source/utils.c -o obj/data_utils.o

lists.o: lists.c
	gcc $(CFLAGS) -c lists.c -o obj/lists.o

clean:
	del main.exe
	del obj\main.o
	del obj\pag_inicio.o
	del obj\pages_methods.o
	del obj\lists.o
	del obj\pag_login.o
	del obj\pag_signup.o
	del obj\data_utils.o
	del obj\user.o
