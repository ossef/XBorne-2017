CFLAGS=-g -O2 -Wall

all: tarjan lump reorder permute

tarjan: tarjan.c io.o tree_v.o sort_v.o 
	gcc $(CFLAGS) -o tarjan io.o tree_v.o sort_v.o tarjan.c -lm

lump: lump.c io.o 
	gcc $(CFLAGS) -o lump io.o lump.c -lm

reorder: reorder.c io.o 
	gcc $(CFLAGS) -o reorder io.o reorder.c -lm

permute: permute.c io.o 
	gcc $(CFLAGS) -o permute io.o permute.c -lm

io.o: io.c io.h struct.h 
	gcc $(CFLAGS) -c io.c -lm

tree_v.o: tree_v.c tree_v.h struct.h
	gcc $(CFLAGS) -c tree_v.c

sort_v.o: sort_v.c sort_v.h struct.h
	gcc $(CFLAGS) -c sort_v.c

clean:
	rm -f *.o	
	rm -f tarjan
	rm -f permute
	rm -f lump
	rm -f reorder
	rm -f *.*.Rii
	rm -f *.*.sz
	rm -f *.*.cd
	rm -f *.*.part
	rm -f *.*.epsi
	rm -f *.*.perm
	rm -f *.EU.*
	rm -f *.EL.*
	rm -f *.EU-L.*
	rm -f *.reorder.*
	rm -f *~


