sand: main.o funcs.o
	g++ main.o funcs.o -o sand -fsanitize=address

main.o: main.cpp
	g++ -c main.cpp -std=c++2a -Og -g -Wall -Wextra

funcs.o: funcs.cpp
	g++ -c funcs.cpp -std=c++2a -Og -g -Wall -Wextra

clean:
	rm *.o sand
