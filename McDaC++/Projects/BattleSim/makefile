Battle_Project: main.o fighter.o start.o battle.o basic.o
	g++ main.o fighter.o start.o battle.o basic.o -o Battle_Project -std=c++17

main.o: main.cpp
	g++ -c main.cpp -std=c++17

fighter.o: fighter.cpp fighter.hpp
	g++ -c fighter.cpp -std=c++17

start.o: start.cpp start.hpp
	g++ -c start.cpp -std=c++17

battle.o: battle.cpp battle.hpp fighter.cpp fighter.hpp
	g++ -c battle.cpp -std=c++17

basic.o: basic.cpp basic.hpp
	g++ -c basic.cpp -std=c++17

clean:
	rm *.o Battle_Project