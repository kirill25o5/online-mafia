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
	int statusID; // 0 - ���������� ���������(����������), 1 - ���� ������, 2 - �������������, 3 - ���� ������ � �������, 
	              //4 - ���� �� �����������, 5-����, 6-������� � ��������


public:
	Player(const std::string& userName, SOCKET ClientSocket, int role, bool isAdmin);
	const std::string& getPlayerName();
	const SOCKET& getSocket();
	bool getIsAdmin();
	int getPlayerRole();
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





