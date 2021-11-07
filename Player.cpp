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

void Player::sleep()
{
	statusID = 5;
}

void Player::awake()
{
	statusID = 0;
}

std::string Player::vote()
{
	std::cout << "enter suspect's name\n";
	std::string suspect;
	std::cin >> suspect;
	return suspect;

}

std::string Player::action()
{
	std::string actionTarget;
	std::cout << "enter player's name";
	std::cin >> actionTarget;
	return actionTarget;

}

const SOCKET& Player::getSocket()
{
	return ClientSocket;
}

int Player::getPlayerRole()
{
	return playerRole;
}
