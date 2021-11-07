
#include "Game.h"

Game::Game(std::vector<Player>& players):players(players) {
	
}

void Game::startGame() {

}

void Game::allSleep()
{
	for (auto player : players) {
		player.setSleepStatus();
	}
}

void Game::night()
{
	allSleep();

}