#include "Shop.h"
#include <iostream>

Shop::Shop()
{
	//Avoid floatping point percision issues by pricing things by
	//cents instead of by dollars
	Menu.insert(MenuItem("Ice Tea", 149));
	Menu.insert(MenuItem("Coffee", 149));
	Menu.insert(MenuItem("Muffin", 269));
	Menu.insert(MenuItem("Croissant", 455));
	Menu.insert(MenuItem("Pound Cake", 679));
	Menu.insert(MenuItem("Sandwhich", 899));
	Menu.insert(MenuItem("Panini", 999));
	Menu.insert(MenuItem("Apple Pie", 1499));
	Menu.insert(MenuItem("Cheesecake", 2499));

	CurrencyTable = new int[4]{ 1, 5, 10, 25};
}

Shop::~Shop()
{
	delete[] CurrencyTable;
}

void Shop::MakeOrder()
{
	std::string Input;

	while (Input != "done")
	{
		std::cout << GetMenuDisplay() << std::endl;
		std::cout << std::endl;

		if (OrderTotal > 0)
		{	
			std::cout << "Subtotal: " << GetCurrencyDisplay(OrderTotal) << std::endl;
			std::cout << std::endl;
		}

		std::cout << R"(Enter the name of another item, or "done" to finish your order.)" << std::endl;
		std::getline(std::cin, Input);

		//case sensitive
		if (Menu.find(Input) != Menu.end())
		{
			OrderTotal += Menu.find(Input)->second;
		}

		//evil
		system("cls");
	}

	std::cout << "Your total is: " << GetCurrencyDisplay(OrderTotal) << std::endl;
	std::cout << "Paying the cashier the nearest dollar amount...\n" << std::endl;

	int Change = ((OrderTotal / 100 + 1) * 100) - OrderTotal;
	std::cout << GetChangeDisplay(Change);
}

std::string Shop::GetMenuDisplay()
{
	std::string Output = "Cafe Menu\n============\n";

	for (MenuItem Item : Menu)
	{
		std::string DecimalString = std::to_string(Item.second);
		DecimalString.insert(DecimalString.end() - 2, '.');
		Output += Item.first + ": \t" + DecimalString + "\n";
	}

	return Output;
}

std::string Shop::GetCurrencyDisplay(int Currency)
{
	std::string Output = "$" + std::to_string(Currency);
	Output.insert(Output.end() - 2, '.');

	if (Output[1] == '.')
	{
		Output.insert(Output.begin() + 1, '0');
	}

	return Output;
}

std::string Shop::GetChangeDisplay(int Change)
{
	int CurrentChange = Change;
	int CurrencyAmount[4] = { 0 };
	int CurrencyIndex = 3;

	while (CurrentChange > 0)
	{
		while (CurrentChange >= CurrencyTable[CurrencyIndex])
		{
			CurrencyAmount[CurrencyIndex]++;
			CurrentChange -= CurrencyTable[CurrencyIndex];
		}

		--CurrencyIndex;
	}

	std::string Output = "You receive " + GetCurrencyDisplay(Change) + " in change\n";
	Output += std::to_string(CurrencyAmount[0]) + " pennies\n";
	Output += std::to_string(CurrencyAmount[1]) + " nickels\n";
	Output += std::to_string(CurrencyAmount[2]) + " dimes\n";
	Output += std::to_string(CurrencyAmount[3]) + " quarters\n";

	return Output;
}

int main()
{
	Shop s;
	s.MakeOrder();

	return 0;
}