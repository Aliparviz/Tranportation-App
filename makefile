CC:= g++
FLAGS = -std=c++11
COMPILE_FLAG = $(FLAGS) -c

utaxi.out: Aplication.o Human.o Location.o Travel.o main.o
	$(CC) Aplication.o Human.o Location.o Travel.o main.o $(FLAGS) -o utaxi.out

Aplication.o: Aplication.cpp Aplication.hpp Travel.hpp Human.hpp Location.hpp
	$(CC) $(COMPILE_FLAG) Aplication.cpp

Human.o: Human.cpp Human.hpp
	$(CC) $(COMPILE_FLAG) Human.cpp

Location.o: Location.cpp Location.hpp Human.hpp
	$(CC) $(COMPILE_FLAG) Location.cpp

Travel.o: Travel.cpp Travel.hpp Human.hpp Location.hpp
	$(CC) $(COMPILE_FLAG) Travel.cpp

main.o: main.cpp Aplication.hpp 
	$(CC) $(COMPILE_FLAG) main.cpp

clean:
	rm *.o
