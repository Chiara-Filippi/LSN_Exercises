CC = g++
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.o random.o randomwalk.o blockav.o
	$(CC) random.o randomwalk.o blockav.o main.o -o main.exe

main.o : main.cpp randomwalk.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)

randomwalk.o : randomwalk.cpp randomwalk.h random.h
	$(CC) -c randomwalk.cpp -o randomwalk.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

blockav.o : blockav.cpp blockav.h
	$(CC) -c blockav.cpp -o blockav.o $(CFLAGS)

clean :
	rm -f *.o main.exe seed.out output_randomwalkcontinuo output_randomwalkdiscreto