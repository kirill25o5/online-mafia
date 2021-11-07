#pragma once
#include <string>
#include <winsock2.h>
#include <iostream>
class Player
{
private:
	SOCKET ClientSocket;
	int playerRole; //0-мирный житель, 1-коммисар, 2-врач,3-мафия
	std::string playerName; //Имя пользователя в мафии
	int playerID;
	int statusID; // 0 - нормальное состояние(бодрствует), 1 - убит мафией, 2 - подозреваемый, 3 - убит мафией и вылечен, 
	              //4 - убит на голосовании, 5-спит, 6-вылечен в холостую


public:
	Player(const std::string& userName, int id, SOCKET ClientSocket, int role);
	const std::string& getPlayerName();
	const SOCKET& getSocket();
	int getPlayerRole();
	int getPlayerID();
	int setSleepStatus();//i5
	int setAwakeStatus();//i0
	int vote(std::string& suspect);
	int action(std::string& actoinTarget);
	int setKilledByMafiaStatus();//i1
	int setSuspectedStatus();//i2
	int setKilledAndHealthedStatus();//i3
	int setHealthed();//i6
	int setKilledByVotingStatus();//i4
};





