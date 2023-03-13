sand: main.o
	g++ main.o -o sand -fsanitize=address

main.o: main.cpp
	g++ -c main.cpp -std=c++2a -Og -g -Wall -Wextra

clean:
	rm *.o sand
