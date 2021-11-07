#include "Player.h"


Player::Player(const std::string &userNmae, int id, SOCKET ClientSocket, int role, bool isAdmin):
	playerName(userNmae), playerID(id), ClientSocket(ClientSocket), statusID(0), playerRole(role), isAdmin(isAdmin)
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
	statusID = 5;
	if (!isAdmin) {
		std::string sendBuf = "i5";
		int iSendResult;
		iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
		if (iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	}
	else
		std::cout << "You're sleeping\n";
	
	return 0;
}

int Player::setAwakeStatus()
{
	statusID = 0;
	if (!isAdmin) {
		std::string sendBuf = "i0";
		int iSendResult;
		iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
		if (iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	}
	else
		std::cout << "You've awaken\n";
	return 0;
}

int Player::vote(std::string& suspect)
{
	if (!isAdmin) {
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
	}
	else {
		std::cout << "Enter name of suspect\n";
		std::cin >> suspect;
	}
	return 0;

}

int Player::action(std::string& actionTarget)
{
	if (!isAdmin) {
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
		delete[] recvBuf;
	}
	else {
		std::cout << "Enter name of Target person";
		std::cin >> actionTarget;
	}
	return 0;

}

const SOCKET& Player::getSocket()
{
	return ClientSocket;
}

bool Player::getIsAdmin()
{
	return isAdmin;
}

int Player::getPlayerRole()
{
	return playerRole;
}

int Player::setKilledByMafiaStatus()
{
	statusID = 1;
	if (!isAdmin) {
		std::string sendBuf = "i1";
		int iSendResult;
		iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
		if (iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	}
	return 0;
}

int Player::setSuspectedStatus()
{
	statusID = 2;
	if (!isAdmin) {
		std::string sendBuf = "i2";
		int iSendResult;
		iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
		if (iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	}
	return 0;
}

int Player::setKilledAndHealthedStatus()
{
	statusID = 3;
	if (!isAdmin) {
		std::string sendBuf = "i3";
		int iSendResult;
		iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
		if (iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	}
	return 0;
}

int Player::setHealthed()
{
	statusID = 6;
	if (!isAdmin) {
		std::string sendBuf = "i6";
		int iSendResult;
		iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
		if (iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	}
	
	return 0;
}

int Player::setKilledByVotingStatus()
{
	statusID = 4;
	if (!isAdmin) {
		std::string sendBuf = "i4";
		int iSendResult;
		iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
		if (iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	}

	return 0;
}