#pragma once
#include <string>
class Player
{
private:
	std::string playerName; //Имя пользователя в мафии
	int playerID;
	int statusID; // 0 - нормальное состояние, 1 - убит мафией, 2 - подозреваемый, 3 - убит мафией и вылечен, 4 - убит на голосовании


public:
	Player(const std::string& userName, int id);
	const std::string& getPlayerName();
	int getPlayerID();
	void sleep();
	void awake();
	void vote();
	friend void setIDasFirst();
};





