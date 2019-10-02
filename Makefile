#tic tac toe makefile

run: tictactoe.o
	g++ tictactoe.o -o run

tictactoe.o: tictactoe.cpp
	g++ -c tictactoe.cpp

clean:
	rm *.o run