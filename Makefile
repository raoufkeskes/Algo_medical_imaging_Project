all: main_test

tomo.o : tomo.c tomo.h
		gcc -c -Wall tomo.c
		
		
main_test.o : main_test.c tomo.h
		gcc -c -Wall main_test.c

	
main_test : main_test.o tomo.o
		gcc -lm -o main_test main_test.o tomo.o

clean	:
	rm -f *.o
	rm -f main_test
