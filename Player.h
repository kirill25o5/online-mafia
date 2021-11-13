#pragma once
#include <string>
#include <winsock2.h>
#include <iostream>
class Player
{
private:
	bool isAdmin;
	SOCKET ClientSocket;
	int playerRole; //0-������ ������, 1-��������, 2-����,3-�����
	std::string playerName; //��� ������������ � �����
	int statusID; //0 - ���������� ��������� (����������), 1 - ���� ������ � �������, 2 - �����, 3 - ����, 4-������� � ��������


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





