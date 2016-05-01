slic_test: slic_test.o slic.o w681388.o slic_test.h
	gcc -Wall -O3 -DSIMULATION -lm slic_test.o slic.o w681388.o -o slic_test

slic_test.o: slic_test.c slic_test.h
	gcc -c -Wall -O3 -DSIMULATION  slic_test.c

slic.o: slic.c slic.h
	gcc -c -Wall -O3 -DSIMULATION slic.c

w681388.o: w681388.c w681388.h w681388Init.h w681388reg.h w681388InitRev.h
	gcc -c -Wall -O3 -DSIMULATION w681388.c

all: slic_test

clean:
	rm -rf slic_test *.o
