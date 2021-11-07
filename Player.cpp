#include "Player.h"


Player::Player(const std::string &userNmae, int id, SOCKET ClientSocket, int role):
	playerName(userNmae), playerID(id), ClientSocket(ClientSocket), statusID(0), playerRole(role)
{}

const std::string& Player::getPlayerName()
{
	return playerName;
}

int Player::getPlayerID()
{
	return playerID;
}

int Player::setSleepStatus()
{
	std::string sendBuf="i5";
	int iSendResult;
	iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	statusID = 5;
	return 0;
}

int Player::setAwakeStatus()
{
	std::string sendBuf = "i0";
	int iSendResult;
	iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	statusID = 0;
	return 0;
}

int Player::vote(std::string& suspect)
{
	std::string sendBuf = "V";
	int iSendResult;
	iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}


	int iResult;
	int recvBuf_len = 255;
	char* recvBuf = new char[recvBuf_len];
	iResult = recv(ClientSocket, recvBuf, recvBuf_len, 0);
	if (iResult > 0) {
		if (recvBuf[0] == 'v') suspect = recvBuf;
		suspect.erase(suspect.begin());
	}
	else {
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	delete[] recvBuf;
	
	return 0;

}

int Player::action(std::string& actionTarget)
{
	std::string sendBuf = "D";
	int iSendResult;
	iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}


	int iResult;
	int recvBuf_len = 255;
	char* recvBuf = new char[recvBuf_len];
	iResult = recv(ClientSocket, recvBuf, recvBuf_len, 0);
	if (iResult > 0) {
		if (recvBuf[0] == 'd') actionTarget = recvBuf;
		actionTarget.erase(actionTarget.begin());
	}
	else {
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	
	return 0;

}

const SOCKET& Player::getSocket()
{
	return ClientSocket;
}

int Player::getPlayerRole()
{
	return playerRole;
}

int Player::setKilledByMafiaStatus()
{
	std::string sendBuf = "i1";
	int iSendResult;
	iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	statusID = 1;
	return 0;
}

int Player::setSuspectedStatus()
{
	std::string sendBuf = "i2";
	int iSendResult;
	iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	statusID = 2;
	return 0;
}

int Player::setKilledAndHealthedStatus()
{
	std::string sendBuf = "i3";
	int iSendResult;
	iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	statusID = 3;
	return 0;
}

int Player::setHealthed()
{
	std::string sendBuf = "i6";
	int iSendResult;
	iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	statusID = 6;
	return 0;
}

int Player::setKilledByVotingStatus()
{
	std::string sendBuf = "i4";
	int iSendResult;
	iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	statusID = 4;

	return 0;
}