
#include "Game.h"
#include <algorithm>

int Game::checkGameStatus()
{
	int mafiaPlayersDead = 0, mafiaPlayerAlive=0;
	int otherPlayersDead = 0, otherPlayersAlive=0;
	for (auto &player : players) {
		if (player.getPlayerRole() == 3 && player.getStatus() != 2) {
			mafiaPlayerAlive++;
		}
		if (player.getPlayerRole() != 3 && player.getStatus() != 2) {
			otherPlayersAlive++;
		}

		
		if (player.getPlayerRole() == 3 && player.getStatus() == 2) {
			mafiaPlayersDead++;
		}
		if (player.getPlayerRole() != 3 && player.getStatus() == 2) {
			otherPlayersDead++;
		}
	}

	if (mafiaPlayersDead == mafia.size()) return 1;
	if (otherPlayersDead == (players.size() - mafia.size()) || (otherPlayersAlive==1 && mafiaPlayerAlive>=otherPlayersAlive)) return 2;
	return 0;
}

void Game::showAdminMorningInfo()
{
	std::cout << "Current players:\n";
	for (auto player : players) {
		std::cout << player.getPlayerName();
		if (player.getStatus() == 2)
			std::cout << ": is dead";
		if (player.getStatus() == 1)
			std::cout << ": was killed by mafia, but then was healthed by doctor";
		if (player.getStatus() == 4)
			std::cout << ": was healthed by doctor";
		std::cout << "\n\n";
	}


	if (lastDeath != -1) {
		std::cout << "Was killed by mafia tonight: "<< players[lastDeath].getPlayerName()<<"\n\n";
	}
	else {
		std::cout << "Nobody was killed by mafia\n\n";
	}
}

void Game::allWakeUp() {
	for (auto &player : players) {
		if (player.getStatus() == 3) {
			player.setAwakeStatus();
		}
	}
}

void Game::sendMorningStatus()
{
	std::string sendBuf = "m";
	for (int i = 0; i < players.size(); ++i) {
		sendBuf += std::to_string(i) + 'i' + std::to_string(players[i].getStatus());
	}


	for (auto &player : players) {
		int iSendResult;
		if (!player.getIsAdmin()) {
			iSendResult = send(player.getSocket(), sendBuf.c_str(), sendBuf.size() + 1, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(player.getSocket());
				WSACleanup();
				return;
			}
		}
	}
}

bool compare(const std::vector<int>& victims) {
	switch (victims.size()) {
	case 0:
		return false;
	case 1:
		return true;
	case 2:
		return (victims[0] == victims[1]);
	case 3:
		return((victims[0] == victims[1]) && (victims[0] == victims[2]) && (victims[1] == victims[2]));

	}
}

int Game::validateNameAndGetID(std::string playerName)
{
	if (playerName.size() == 1 && playerName.front() >= '0' && playerName.front() <= '7') {
		return atoi(playerName.c_str());
	}

	auto playerID = std::find_if(players.begin(), players.end(), [playerName](Player& player) {
		return playerName == player.getPlayerName();
		});

	if ( playerID != players.end()) {
		return std::distance(players.begin(), playerID);
	}

	return 100;
}

int Game::voting()
{
	std::vector<int> votes;
	std::string vote;
	std::vector<int>::iterator max;
	do {
		votes.assign(players.size(), 0);
		for (auto &player : players) {
			if (player.getStatus() == 2) continue;
			do {
				player.vote(vote);
			} while (validateNameAndGetID(vote)==100);
			votes[validateNameAndGetID(vote)] ++;
		}
		max = std::max_element(votes.begin(), votes.end());
	} while (std::find(max + 1, votes.end(), *max) != votes.end());

	players[std::distance(votes.begin(), max)].setDeadStatus();

	return std::distance(votes.begin(), max);
	
}

Game::Game(std::vector<Player>& players):players(players), police(-1), doctor(-1), lastDeath(-1){
	for (int i = 0; i < players.size(); ++i) {
		switch (players[i].getPlayerRole())
		{
		case 1:
			police = i;
			break;
		case 2:
			doctor = i;
			break;
		case 3:
			mafia.push_back(i);
			break;
		default:
			break;
		}
	}



}

void Game::startGame() {
	while(checkGameStatus()==0) {
		night();
		if (checkGameStatus() == 2) break;
		day();
	}
	if (checkGameStatus() == 1) std::cout << "others win";
	else std::cout << "mafia win";
}

void Game::allSleep()
{
	for (auto &player : players) {
		if(player.getStatus()!=2)
			player.setSleepStatus();
	}
}

int Game::mafiaAction()
{
	
	std::vector<int> victims;
	while (!compare(victims)) {
		victims.clear();

		std::string victim;
		for (auto mafiaID : mafia) {
			if (players[mafiaID].getStatus() == 2) continue;
			players[mafiaID].action(victim);
			while (validateNameAndGetID(victim) == 100)
				players[mafiaID].action(victim);

			victims.push_back(validateNameAndGetID(victim));
		}
	}


	return victims.front();
}

int Game::policeAction()
{

	if (players[police].getStatus() == 2 || police == -1) return 100;
	std::string suspect_str;
	int suspect;
	do {
		players[police].action(suspect_str);
		suspect = validateNameAndGetID(suspect_str);
	} while (suspect==100);


	std::string sendBuf;
	if (players[suspect].getPlayerRole() == 3)
		sendBuf = '1';
	else
		sendBuf = '0';

	if (!players[police].getIsAdmin()) {
		int iSendResult;
		iSendResult = send(players[police].getSocket(), sendBuf.c_str(), sendBuf.size() + 1, 0);
		if (iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(players[police].getSocket());
			WSACleanup();
			return 1;
		}
	}
	else {
		std::cout << sendBuf<<"\n";
	}

	return suspect;
}

int Game::doctorAction()
{
	if (doctor == -1 || players[doctor].getStatus()==2) {
		return 100;
	}

	std::string cured;
	do {
		players[doctor].action(cured);
	} while (validateNameAndGetID(cured) == 100);


	return validateNameAndGetID(cured);
}

void Game::night()
{
	allSleep();
	system("pause 1000");

	int killedByMafiaID = mafiaAction();
	int suspectID = policeAction();
	int curedID = doctorAction();
	

	lastDeath = -1;
	if (killedByMafiaID == curedID && killedByMafiaID!=100) 
		players[killedByMafiaID].setKilledAndHealthedStatus();
	else {
		if(killedByMafiaID!=100) players[killedByMafiaID].setDeadStatus();
		if(curedID!=100) players[curedID].setHealthed();
		lastDeath = killedByMafiaID;
	}
}

void Game::day()
{
	allWakeUp();

	sendMorningStatus();
	system("pause 1000");

	showAdminMorningInfo();

	std::cout << "\nVoting is started\n";

	int dead = voting();

	std::cout << players[dead].getPlayerName() << " was killed by voting\n";

	std::string sendBuf = "k"+std::to_string(dead);
	for (auto &player : players) {
		int iSendResult;
		if (!player.getIsAdmin()) {
			iSendResult = send(player.getSocket(), sendBuf.c_str(), sendBuf.size() + 1, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(player.getSocket());
				WSACleanup();
				return;
			}
		}
	}

}