#include "Euchre.h"
#include <iostream>
#include <string>

Euchre::Euchre()
	: Players(4)
	, Dealer(0)
	, CurrentPlayer(0)
	, Trump(Suit::Clubs)
{	
	Players[0].Teammate = &Players[2];
	Players[1].Teammate = &Players[3];
	Players[2].Teammate = &Players[0];
	Players[3].Teammate = &Players[1];
}

void Euchre::Play()
{
	int TeamWins = 0;

	for (int i = 0; i < 100; ++i)
	{
		Deal();
		DetermineTrump();

		//First player to make a move is the player after the dealer
		CurrentPlayer = GetNextPlayerIndex(Dealer);

		//Play 5 tricks for every hand
		//We only track tricks one for team 1 (player 0 and player 2)
		//Because we can just subtract team 1 tricks from 5 to get
		//wins for team 2
		int TricksWon = 0;

		for (int j = 0; j < 5; ++j)
		{
			if (PlayTrick())
			{
				++TricksWon;
			}
		}

		if (TricksWon > 2)
		{
			std::string output = "Team 1 wins hand " + std::to_string(i + 1) + " with " + std::to_string(TricksWon) + " tricks won.";

			if (Players[0].bSittingOut || Players[2].bSittingOut)
			{
				output += " This hand was won when going alone.";
			}

			std::cout << output << std::endl;
			TeamWins++;
		}

		else
		{
			std::string output = "Team 2 wins hand " + std::to_string(i + 1) + " with " + std::to_string(5 - TricksWon) + " tricks won.";

			if (Players[1].bSittingOut || Players[3].bSittingOut)
			{
				output += " This hand was won when going alone.";
			}

			std::cout << output << std::endl;
		}

		for (int j = 0; j < 4; ++j)
		{
			Players[j].bSittingOut = false;
			Players[j].Hand.clear();
		}

		//Next player becomes dealer
		Dealer = GetNextPlayerIndex(Dealer);
	}

	std::cout << "" << std::endl;
	std::cout << "After 100 rounds, Team 1 won " << TeamWins << " hands and Team 2 won " << 100 - TeamWins << " hands." << std::endl;

}

void Euchre::Deal()
{
	CardDeck.Shuffle();
	CurrentPlayer = GetNextPlayerIndex(Dealer);

	int i = 0;
	while (i < 20)
	{	
		for (int j = 0; j < 8; ++j)
		{
			//Deal 3 to odd number players first pass, Deal 3 to even number players second pass
			const int DealSize = (j > 3 ? (j % 2 == 0 ? 2 : 3) : (j % 2 == 1 ? 2 : 3));

			for (int k = 0; k < DealSize; k++)
			{
				Players[CurrentPlayer].Hand.push_back(CardDeck[i++]);
			}
			
			CurrentPlayer = GetNextPlayerIndex(CurrentPlayer);
		}
	}

	Trump = CardDeck[i]->GetSuit();
}

void Euchre::DetermineTrump()
{
	for (int i = 0; i < 4; ++i)
	{
		Player& Player = Players[CurrentPlayer];

		switch (Player.GetPlayRequest(Trump))
		{
			//If the player wants to take, just make the dealer swap the card
		case Take:
			Players[Dealer].SwapCard(CardDeck[20], Trump);
			return;

			//Decide to play alone, tell teammate to sit out this hand
		case Alone:
			Player.Teammate->bSittingOut = true;
			Players[Dealer].SwapCard(CardDeck[20], Trump);
			return;

			//otherwise just move to the next player 
		default:
			break;
		}

		CurrentPlayer = GetNextPlayerIndex(CurrentPlayer);
	}

	//If no player wnats the card, force the dealer to decide
	Trump = Players[Dealer].GetDesiredSuit();
	Players[Dealer].SwapCard(CardDeck[20], Trump);
}

int Euchre::GetNextPlayerIndex(int Current) const
{
	if (Current < 0 && Current >= Players.size())
	{
		throw std::out_of_range("Bad value");
	}

	if (Current == Players.size() - 1)
	{
		return 0;
	}

	return ++Current;
}

bool Euchre::PlayTrick()
{
	bool bFirstPlay = true;
	Suit LeadSuit;

	//Cards that were played this hand
	const Card* PlayedCards[4];
	
	//note: this implementation is lazy. it doesn't take into account cards that have been played
	//in previous tricks which we could use as information to make better decisions
	for (int i = 0; i < 4; ++i)
	{
		PlayedCards[i] = Players[CurrentPlayer].GetPlayCard(bFirstPlay, LeadSuit, PlayedCards, i, Trump);
		bFirstPlay = false;
		CurrentPlayer = GetNextPlayerIndex(CurrentPlayer);
	}

	//Once all players play a card, determine which player played the winning card
	int WinningPlayerIndex = GetWinningCardIndex(PlayedCards, LeadSuit, CurrentPlayer);

	//Player who won the trick goes first next turn
	CurrentPlayer = WinningPlayerIndex;

	return WinningPlayerIndex == 0 || WinningPlayerIndex == 2;
}

int Euchre::GetWinningCardIndex(const const Card** PlayedCards, const Suit LeadSuit, int FromPlayer) const
{
	int BestValue = -1;
	int BestCardIndex = -1;

	for (int i = 0; i < 4; ++i)
	{
		int Value = -1;

		if (PlayedCards[i]->GetSuit() == Trump)
		{
			Value = PlayedCards[i]->GetValue() + 6; //Add extra weight to trump cards

			//Right hand jack is best card
			if (Value == 11)
			{
				Value = 12;
			}
		}

		else if (PlayedCards[i]->GetSuit() == LeadSuit)
		{
			Value = PlayedCards[i]->GetValue();
		}

		//Left hand jack is second most valuable
		else if (PlayedCards[i]->GetValue() && PlayedCards[i]->GetSuit() % 2 == Trump % 2)
		{
			Value = 11;
		}

		else
		{
			//Non trump/lead/or left hand jacks can ever win, so don't even bother computing their value
		}

		if (Value > BestValue)
		{
			BestValue = Value;
			BestCardIndex = i;
		}
	}

	FromPlayer += BestCardIndex;
	if (FromPlayer > 3)
	{
		FromPlayer -= 4;
	}

	return FromPlayer;
}

int main()
{
	Euchre game;
	game.Play();
	return 0;
}
