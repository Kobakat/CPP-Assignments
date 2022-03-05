#pragma once
#include <vector>

class Card;
enum Suit;

enum PlayType { Pass, Take, Alone };

class Player
{
	friend class Euchre;

public:
	Player();

private:

	Player* Teammate;

	std::vector<const Card*> Hand;

	bool bSittingOut : 1;

public:

	/** Returns the peferred play type of the player on this round*/
	PlayType GetPlayRequest(const Suit suit) const;

	/** If no player wants the selected trump card, the dealer calls this to determine what suit to use*/
	Suit GetDesiredSuit() const;

	/** When the trump suit is selected, dealer calls this to swap the face up card with one of theirs*/
	void SwapCard(const Card* NewCard, const Suit Trump);

	/** Select the card to be played this trick*/
	const Card* GetPlayCard(const bool bFirstPlay, Suit& LeadSuit, const const Card** PlayedCards, const int Length, const Suit TrumpSuit);

private:

	/** Returns a copy to the card at the provided index while also removing it from the hand*/
	const Card* PlayCard(const int Index);

	/** Returns true if the player has a card in their hand of the provided suit*/
	bool HasCardOfSuit(const Suit Suit) const;

	/** Returns true if the player has the left jack in their hand*/
	bool HasLeftJack(const Suit TrumpSuit) const;

	/** Returns the index of a card in the hand that can beat the best played card. Returns -1 if one can't be found*/
	int GetBetterCardIndex(const Suit LeadSuit, const const Card** PlayedCards, const int Length, const Suit TrumpSuit) const;

};

