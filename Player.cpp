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

const SOCKET& Player::getSocket()
{
	return ClientSocket;
}

int Player::getPlayerRole()
{
	return playerRole;
}
