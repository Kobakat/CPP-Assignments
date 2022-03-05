#pragma once

enum Suit { Spades, Hearts, Diamonds, Clubs, Num /*sneaky trick*/ };

class Card
{
	friend class Deck;

public:

	inline Suit GetSuit() const { return Suit; }
	inline int GetValue() const { return Value; }

private:

	int Value;
	Suit Suit;
};

