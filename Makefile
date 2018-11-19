all: compil

compil: main.cpp
	g++ --std=c++11 main.cpp -o mySuperGame

clear:
	rm mySuperGame
