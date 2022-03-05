#pragma once

#include <stdexcept>
#include "Card.h"
#include <iostream>

class Deck
{
public:

	Deck();
	~Deck();

	const Card* operator[](int i) { return &Cards[i]; }

private:

	/*no need for dynamic resize so use standard array */
	Card* Cards;

public:

	void Shuffle();	
};
