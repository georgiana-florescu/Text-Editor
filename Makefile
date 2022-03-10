build: editor

editor: main.o  stack.o double_linked_list.o
	gcc main.o  stack.o double_linked_list.o -o editor

main.o: main.c

stack.o: stack.h stack.c

double_linked_list.o: double_linked_list.h double_linked_list.c

run:
	./editor

memcheck:
	valgrind ./editor

make clean:
	rm -f *.o editor

