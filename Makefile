CFLAG = -DDEBUG -Wall -std=c99

main: main.o my_scheduler.o my_process.o
	gcc $(CFLAG) main.o my_scheduler.o my_process.o -o main
main.o: main.c Makefile
	gcc $(CFLAG) main.c -c
my_scheduler.o: my_scheduler.c my_scheduler.h Makefile
	gcc $(CFLAG) my_scheduler.c -c
my_process.o: my_process.c my_process.h Makefile
	gcc $(CFLAG) my_process.c -c
clean:
	rm -rf *o
run:
	sudo ./main