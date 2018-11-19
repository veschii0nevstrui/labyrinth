all: compil

compil: main.cpp
	g++ --std=c++11 main.cpp -o MySuperGame

clear:
	rm MySuperGame
