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
	std::vector<short> mafia;
	short police;
	short doctor;
	short lastDeath;


public:
	Game(std::vector<Player>& players);
	void startGame();
	void night();
	void allSleep();
	void allWakeUp();
	void day();
	void sendMorningStatus();
	void showAdminMorningInfo();
	int checkGameStatus();
	int voting();
	int mafiaAction();
	int policeAction();
	int doctorAction();
	int validateNameAndGetID(std::string playerName);
};

bool compare(const std::vector<int>& victims);
