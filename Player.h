#pragma once
#include <string>
#include <winsock2.h>
class Player
{
private:
	SOCKET ClientSocket;
	int playerRole; //0-������ ������, 1-��������, 2-����,3-�����
	std::string playerName; //��� ������������ � �����
	int playerID;
	int statusID; // 0 - ���������� ���������, 1 - ���� ������, 2 - �������������, 3 - ���� ������ � �������, 4 - ���� �� �����������


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





