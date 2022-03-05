#pragma once

#include "Player.h"
#include "Deck.h"
#include <vector>

/** Driver class for game state management */
class Euchre
{
public:
	Euchre();

private:
	std::vector<Player> Players;

	Deck CardDeck;

	/** Index of current player designated the dealer*/
	int Dealer;

	/** Index of the player who's turn it is to make a move*/
	int CurrentPlayer;

public:

	/** Starts the game*/
	void Play();

private:

	/** Deals a hand to every player in Euchre format*/
	void Deal();

	/** Asks every player if they want the trump suit, forcing the dealer to choose if everyone declines*/
	void DetermineTrump();

	/** Returns the player that comes after Current*/
	int GetNextPlayerIndex(int Current) const;

	/** Plays a single trick. Returns true if team 1 wins and false if 2 team wins*/
	bool PlayTrick();

	int GetWinningCardIndex(const const Card** PlayedCards, const Suit LeadSuit, int FromPlayer) const;

	/** Current trump suit*/
	Suit Trump;
};
