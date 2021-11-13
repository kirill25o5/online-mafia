#pragma once
#include <string>
#include <winsock2.h>
#include <iostream>
class Player
{
private:
	bool isAdmin;
	SOCKET ClientSocket;
	int playerRole; //0-мирный житель, 1-коммисар, 2-врач,3-мафия
	std::string playerName; //Имя пользователя в мафии
	int statusID; //0 - нормальное состояние (бодрствует), 1 - убит мафией и вылечен, 2 - мертв, 3 - спит, 4-вылечен в холостую


public:
	Player(const std::string& userName, SOCKET ClientSocket, int role, bool isAdmin);
	const std::string& getPlayerName();
	const SOCKET& getSocket();
	bool getIsAdmin();
	int getPlayerRole();
	int getStatus();
	int setSleepStatus();//i3
	int setAwakeStatus();//i0
	int vote(std::string& suspect);
	int action(std::string& actoinTarget);
	int setKilledAndHealthedStatus();//i1
	int setHealthed();//i4
	int setDeadStatus();//i2
};





