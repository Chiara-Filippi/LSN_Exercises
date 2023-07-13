CC = g++
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.o random.o european.o blockav.o
	$(CC) random.o european.o blockav.o main.o -o main.exe

main.o : main.cpp european.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)

european.o : european.cpp european.h random.h
	$(CC) -c european.cpp -o european.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

blockav.o : blockav.cpp blockav.h random.h
	$(CC) -c blockav.cpp -o blockav.o $(CFLAGS)

clean :
	rm -f *.o main.exe seed.out diretto_* discreto_*