#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>

typedef std::pair<const std::string, int> MenuItem;

class Shop
{
public:

	Shop();
	~Shop();

	/** List of menu items*/
	std::map<std::string, int> Menu;

	void MakeOrder();

private:

	/** Returns the display string of the menu*/
	std::string GetMenuDisplay();

	/** Returns the display string of the current order*/
	std::string GetCurrencyDisplay(int Currency);

	/** Returns the display string of the change received after paying for an order*/
	std::string GetChangeDisplay(int Change);

	/** Current total of the customer order*/
	int OrderTotal;

	/** Lookup table for change back*/
	int* CurrencyTable;
};