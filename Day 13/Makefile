ds: distress_signal.o
	g++ distress_signal.o -o ds -fsanitize=address

distress_signal.o: distress_signal.cpp
	g++ -c distress_signal.cpp -std=c++2a -Og -g

clean:
	rm *.o ds
