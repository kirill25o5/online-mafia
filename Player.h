#pragma once
#include <string>
#include <winsock2.h>
class Player
{
private:
	SOCKET ClientSocket;
	std::string playerName; //Имя пользователя в мафии
	int playerID;
	int statusID; // 0 - нормальное состояние, 1 - убит мафией, 2 - подозреваемый, 3 - убит мафией и вылечен, 4 - убит на голосовании


public:
	Player(const std::string& userName, int id, SOCKET ClientSocket);
	const std::string& getPlayerName();
	const SOCKET& getSocket();
	int getPlayerID();
	void sleep();
	void awake();
	void vote();
	friend void setIDasFirst();
};





