#include "Player.h"
#include "Card.h"
#include <iostream>

Player::Player()
	: Teammate(nullptr)
{
	Hand.reserve(5);
}

PlayType Player::GetPlayRequest(Suit Suit) const
{
	/**
	 *	In a given deal, on average, 5 cards of a given suit are dealt.
	 *	Given that the left hand jack is the second most valuable card,
	 *  and is dealt in 20/24 hands, we should always consider 6 cards 
	 *	as relevant to this hand. Considering there are 4 players. On
	 *	average, 1.5 "valuable" cards are dealt. If the player has at 
	 *	least 2 cards that belong to the suit (or the left hand jack), 
	 *	they will decide to use this suit, if they have 4 or more cards
	 *	they will go alone.
	 */

	int ValuableCards = 0;

	for (const Card* Card : Hand)
	{
		//Trump card
		if (Card->GetSuit() == Suit)
		{
			++ValuableCards;
		}

		//left hand jack check. this works because suit enum is intentionally staggered
		else if ((Card->GetValue() == 5) && (Card->GetSuit() % 2 == Suit % 2))
		{
			++ValuableCards;
		}
	}

	return static_cast<PlayType>(std::min(ValuableCards / 2, 2));
}

Suit Player::GetDesiredSuit() const
{
	int SuitTable[4] = { 0 };
	int Choice = -1;

	//lazy implementation. doesn't take into account having 2 cards of the same suit.
	//in that case you would want to select which suit you have a higher value of.
	for (const Card* Card : Hand)
	{
		const int Suit = Card->GetSuit();
		SuitTable[Suit]++;

		if (SuitTable[Suit] > Choice)
		{
			Choice = Suit;
		}
	}

	return static_cast<Suit>(Choice);
}

void Player::SwapCard(const Card* NewCard, const Suit Trump)
{
	int WorstValue = INT_MAX;
	int WorstCardIndex = 0;

	for (int i = 0; i < Hand.size(); ++i)
	{		
		int Value = 0;

		//If this card is the trump suit the value is increased above all other cards
		if (Trump == Hand[i]->GetSuit())
		{
			Value = Hand[i]->GetValue() + 6;
		}
		
		//If this card is is the left hand jack, also apply the multplier. Due to the weight
		//of the card, it doesn't matter that this has the same value as the the right hand jack
		//because the player will never want to discard it either if they draw both.
		else if(Hand[i]->GetValue() == 5 && Hand[i]->GetSuit() % 2 == Trump % 2)
		{
			Value = Hand[i]->GetValue() + 6;
		}

		//Normal card, compute the value as normal
		else
		{
			Value = Hand[i]->GetValue();
		}

		//Now keep track of the worst card in the hand
		if (Value < WorstValue)
		{
			WorstValue = Value;
			WorstCardIndex = i;
		}
	}

	//Now just swap the card for the worst card
	Hand[WorstCardIndex] = NewCard;
}

//double const madness
const Card* Player::GetPlayCard(const bool bFirstPlay, Suit& LeadSuit, const const Card** PlayedCards, const int Length, const Suit TrumpSuit)
{
	if (bFirstPlay)
	{
		//If we have the right jack, we can force everyone to play trump suit cards but still take a win.
		for (int i = 0; i < Hand.size(); ++i)
		{
			if (Hand[i]->GetValue() == 5 && Hand[i]->GetSuit() == TrumpSuit)
			{
				LeadSuit = Hand[i]->GetSuit();
				return PlayCard(i);
			}
		}

		// If we don't have the best card, it's more likely one of the opponents has it and our teammate does not
		// Instead just play the best card of a different suit
		int BestValue = -1;
		int BestCardIndex = -1;

		//During this loop keep track of trump card weights so we can handle edge cases where we only have trump cards
		//in a single pass
		int BestTrumpValue = -1;
		int BestTrumpCardIndex = -1;
		bool bOnlyHasTrumpCards = true;

		for (int i = 0; i < Hand.size(); ++i)
		{
			int Value = 0;

			if (TrumpSuit != Hand[i]->GetSuit())
			{
				bOnlyHasTrumpCards = false;

				//Left hand jack, is higher weighted
				if (Hand[i]->GetValue() == 5 && Hand[i]->GetSuit() % 2 == TrumpSuit % 2)
				{
					Value = Hand[i]->GetValue() + 6;
				}

				//Normal card, compute the value as normal
				else
				{
					Value = Hand[i]->GetValue();
				}

				//Keep track of the best card in the hand
				if (Value > BestValue)
				{
					BestValue = Value;
					BestCardIndex = i;
				}
			}

			else
			{
				Value = Hand[i]->GetValue() + 6;

				if (Value > BestTrumpValue)
				{
					BestTrumpValue = Value;
					BestTrumpCardIndex = i;
				}
			}
		}

		if (!bOnlyHasTrumpCards)
		{
			LeadSuit = Hand[BestCardIndex]->GetSuit();
			return PlayCard(BestCardIndex);
		}

		else
		{
			LeadSuit = TrumpSuit;
			return PlayCard(BestTrumpCardIndex);
		}
	}

	else
	{
		//If we have a card of the lead suit we have to play it
		if (HasCardOfSuit(LeadSuit))
		{
			//Determine if we can beat the currently best played card
			int BetterCard = GetBetterCardIndex(LeadSuit, PlayedCards, Length, TrumpSuit);

			//This implementation is a bit lazy. We aren't taking into account our teammate at all. In some cases
			//they may have played a winning card already and we'd be wasting a potentially higher value card
			//when we did not need to
			if (BetterCard >= 0)
			{
				return PlayCard(BetterCard);
			}

			//Otherwise just play the worst card we have
			else
			{
				int WorstValue = INT_MAX;
				int WorstCardIndex = -1;

				for (int i = 0; i < Hand.size(); ++i)
				{
					int Value = Hand[i]->GetValue();

					if (Value < WorstValue)
					{
						WorstValue = Value;
						WorstCardIndex = i;
					}
				}

				return PlayCard(WorstCardIndex);
			}
		}

		else
		{
			//If the lead suit isn't trump, play the lowest value trump card to try and sneak a win
			if (HasCardOfSuit(TrumpSuit))
			{
				int WorstValue = INT_MAX;
				int WorstCardIndex = 0;

				for (int i = 0; i < Hand.size(); ++i)
				{
					if (Hand[i]->GetSuit() == TrumpSuit)
					{
						int Value = Hand[i]->GetValue();

						if (Value < WorstValue)
						{
							WorstValue = Value;
							WorstCardIndex = i;
						}
					}		
				}

				return PlayCard(WorstCardIndex);
			}

			//If we don't have a trump card, look to play the left jack
			else if(HasLeftJack(TrumpSuit))
			{
				for (const Card* Card : Hand)
				{
					auto It = std::find(Hand.begin(), Hand.end(), Card);

					if (It != Hand.end())
					{
						int Index = It - Hand.begin();
						return PlayCard(Index);
					}
				}
			}

			//Otherwise just play the lowest value card we have
			else
			{
				int WorstValue = INT_MAX;
				int WorstCardIndex = -1;

				for (int i = 0; i < Hand.size(); ++i)
				{
					int Value = Hand[i]->GetValue();

					if (Value < WorstValue)
					{
						WorstValue = Value;
						WorstCardIndex = i;
					}
				}

				return PlayCard(WorstCardIndex);
			}
		}
	}
}

const Card* Player::PlayCard(const int Index)
{
	const Card* Temp = Hand[Index];
	Hand.erase(Hand.begin() + Index);
	return Temp;
}

bool Player::HasCardOfSuit(const Suit Suit) const
{
	for (const Card* Card : Hand)
	{
		if (Card->GetSuit() == Suit)
		{
			return true;
		}
	}

	return false;
}

bool Player::HasLeftJack(const Suit TrumpSuit) const
{
	for (const Card* Card : Hand)
	{
		if ((Card->GetValue() == 5) && (Card->GetSuit() % 2 == TrumpSuit % 2))
		{
			return true;
		}
	}
	
	return false;
}

int Player::GetBetterCardIndex(const Suit LeadSuit, const const Card** PlayedCards, const int Length, const Suit TrumpSuit) const
{
	int BestCardValue = -1;

	for (int i = 0; i < Length; ++i)
	{
		int Value = 0;
		
		if (PlayedCards[i]->GetSuit() == TrumpSuit)
		{
			Value = PlayedCards[i]->GetValue() + 6;
		}

		else if (PlayedCards[i]->GetValue() == 5 && PlayedCards[i]->GetSuit() % 2 == TrumpSuit % 2)
		{
			Value = PlayedCards[i]->GetValue() + 6;
		}

		else
		{
			Value = PlayedCards[i]->GetValue();
		}

		if (Value > BestCardValue)
		{
			BestCardValue = Value;
		}
	}

	//Now that we know the best card, compute if we can beat it
	for (int i = 0; i < Hand.size(); ++i)
	{
		int Value = 0;

		if (Hand[i]->GetSuit() == TrumpSuit)
		{
			Value = Hand[i]->GetValue() + 6;
		}

		else if (Hand[i]->GetValue() && Hand[i]->GetSuit() % 2 == TrumpSuit % 2)
		{
			Value = Hand[i]->GetValue() + 6;
		}

		else
		{
			Value = Hand[i]->GetValue();
		}

		if (Value > BestCardValue)
		{
			return i;
		}
	}

	return -1;
}
