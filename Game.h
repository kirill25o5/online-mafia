#pragma once

#include <string>
#include <winsock2.h>
#include <iostream>
#include <vector>
#include "Player.h"

class Game
{
private:
	std::vector<Player> players;


public:
	Game(std::vector<Player>& players);
	void startGame();
	void night();
	void allSleep();
};