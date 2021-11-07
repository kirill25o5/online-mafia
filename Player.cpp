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

int Player::sleep()
{
	int iSendResult;
	iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size() + 1, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	statusID = 5;
}

int Player::awake()
{
	statusID = 0;
}

int Player::vote(std::string& suspect)
{
	std::cout << "enter suspect's name\n";
	std::cin >> suspect;
	return 0;

}

int Player::action(std::string& actionTarget)
{
	std::cout << "enter player's name";
	std::cin >> actionTarget;
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
