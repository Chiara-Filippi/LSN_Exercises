CC = g++
CFLAGS = -Wall -O3 --std=c++11

main2.exe: main2.o random.o blockav.o
	$(CC) random.o main2.o blockav.o -o main2.exe

main2.o: main2.cpp
	$(CC) -c main2.cpp -o main2.o $(CFLAGS)

random.o: random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

blockav.o: blockav.cpp blockav.h
	$(CC) -c blockav.cpp -o blockav.o $(CFLAGS)

clean:
	rm *.o main2.exe seed.out