CC = mpic++
CFLAGS = -Wall -O3 --std=c++11

main.exe: main.o random.o Map.o Route.o Population.o
	$(CC) random.o main.o Map.o Route.o Population.o -o main.exe

main.o: main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)

random.o: random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

Map.o: Map.cpp Map.h
	$(CC) -c Map.cpp -o Map.o $(CFLAGS)

Route.o: Route.cpp Route.h
	$(CC) -c Route.cpp -o Route.o $(CFLAGS)

Population.o: Population.cpp Population.h
	$(CC) -c Population.cpp -o Population.o $(CFLAGS)

clean:
	rm *.o main.exe seed.out *_Square *_Circle