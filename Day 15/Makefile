beacon: main.o funcs.o
	g++ main.o funcs.o -o beacon -fsanitize=address

main.o: main.cpp
	g++ -c main.cpp -std=c++20 -Og -g -Wall -Wextra

funcs.o: funcs.cpp
	g++ -c funcs.cpp -std=c++20 -Og -g -Wall -Wextra

clean:
	rm *.o beacon
