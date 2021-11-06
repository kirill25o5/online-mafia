#pragma once
#include <string>
#include <winsock2.h>
class Player
{
private:
	SOCKET ClientSocket;
	int playerRole; //0-мирный житель, 1-коммисар, 2-врач,3-мафия
	std::string playerName; //Имя пользователя в мафии
	int playerID;
	int statusID; // 0 - нормальное состояние, 1 - убит мафией, 2 - подозреваемый, 3 - убит мафией и вылечен, 4 - убит на голосовании


public:
	Player(const std::string& userName, int id, SOCKET ClientSocket, int role);
	const std::string& getPlayerName();
	const SOCKET& getSocket();
	int getPlayerID();
	void sleep();
	void awake();
	void vote();
	friend void setIDasFirst();
};





