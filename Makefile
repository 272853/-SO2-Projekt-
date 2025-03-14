program: main.o firstSO.o
	g++ main.o firstSO.o -o program

main.o: main.cpp firstSO.h
	g++ -c main.cpp

firstSO.o: firstSO.cpp firstSO.h
	g++ -c firstSO.cpp

clean:
	rm -f *.o program
