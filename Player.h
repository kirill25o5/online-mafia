#pragma once
#include <string>
class Player
{
private:
	std::string playerName; //��� ������������ � �����
	int playerID;
	int statusID; // 0 - ���������� ���������, 1 - ���� ������, 2 - �������������, 3 - ���� ������ � �������, 4 - ���� �� �����������


public:
	Player(const std::string& userName, int id);
	const std::string& getPlayerName();
	int getPlayerID();
	void sleep();
	void awake();
	void vote();
	friend void setIDasFirst();
};





