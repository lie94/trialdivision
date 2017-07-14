FLAGS= -std=c++11 -Wall

CC=g++

OBJ =  threadhandler.o trial.o util.o listtrial.o

.SECONDARY: $(OBJ)

all: timetest.out primelist.tmp main.out

primelist.tmp : sieve/sieve.out
	./sieve/sieve.out

sieve/sieve.out : sieve/sieve.cpp
	$(CC) $(FLAGS) sieve/sieve.cpp settings.h -o sieve/sieve.out

%.o: %.cpp %.h settings.h
	$(CC) $(FLAGS) $^ -c

%.out : %.cpp $(OBJ) settings.h
	$(CC) $(FLAGS) $^ -pthread -lgmp -o $@

vtimetest.out : timetest.cpp trial.cpp util.cpp threadhandler.cpp settings.h
	$(CC) $(FLAGS) -g timetest.cpp trial.cpp util.cpp threadhandler.cpp -pthread -lgmp threadhandler.h settings.h trial.h util.h  -o vtimetest.out

valgrind : vtimetest.out
	valgrind --track-origins=yes --leak-check=yes ./vtimetest.out NM 0 200 1 100

clean :
	rm -f *.out
	rm -f *.tmp
	rm -f *.o
	rm -f *.h.gch
