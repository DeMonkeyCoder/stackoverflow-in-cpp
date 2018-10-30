run : main.o 
	./run
main.o : 
	g++ main.cpp -o run -std=c++11
