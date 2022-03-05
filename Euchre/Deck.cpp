#include "Deck.h"
#include "Card.h"
#include <random>
#include <time.h>
#include <iostream>

constexpr int DECK_SIZE = 24;
constexpr int CARDS_PER_SUIT = 6;

Deck::Deck()
{ 
	Cards = new Card[DECK_SIZE];
	srand(time(nullptr));

	for (int i = 0; i < Suit::Num; ++i)
	{
		for (int j = 0; j < CARDS_PER_SUIT; ++j)
		{
			int Index = CARDS_PER_SUIT * i + j;
			Cards[Index].Value = j;
			Cards[Index].Suit = static_cast<Suit>(i);
		}
	}
}

Deck::~Deck()
{
	delete[] Cards;
}

void Deck::Shuffle()
{
	for (int i = DECK_SIZE - 1; i > 0; --i)
	{
		const int Selection = rand() % i;

		Card Temp = Cards[i];
		Cards[i] = Cards[Selection];
		Cards[Selection] = Temp;	
	}
}