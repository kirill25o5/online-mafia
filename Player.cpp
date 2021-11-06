#include "Player.h"

Player::Player(const std::string &userNmae, int id):playerName(userNmae), playerID(id)
{}

const std::string& Player::getPlayerName()
{
	return playerName;
}

int Player::getPlayerID()
{
	return playerID;
}
