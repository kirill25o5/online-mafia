#pragma once
#include <string>
#include <winsock2.h>
#include <iostream>
class Player
{
private:
	SOCKET ClientSocket;
	int playerRole; //0-������ ������, 1-��������, 2-����,3-�����
	std::string playerName; //��� ������������ � �����
	int playerID;
	int statusID; // 0 - ���������� ���������(����������), 1 - ���� ������, 2 - �������������, 3 - ���� ������ � �������, 
	              //4 - ���� �� �����������, 5-����, 6-������� � ��������


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





