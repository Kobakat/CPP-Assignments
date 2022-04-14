#include <iostream>
#include <string>

std::string Multiply(std::string num1, std::string num2)
{
    const std::string zero = "0";

    if (num1 == zero || num2 == zero)
    {
        return zero;
    }

    int* buffer = new int[num1.size() + num2.size()]{ 0 };

    for (int i = num1.size() - 1; i >= 0; i--)
    {
        for (int j = num2.size() - 1; j >= 0; j--)
        {
            int product = (num1[i] - '0') * (num2[j] - '0');
            int current = product + buffer[i + j + 1];
            buffer[i + j + 1] = current % 10;
            buffer[i + j] += current / 10;
        }

    }

    int i = 0;
    while (i < num1.size() + num2.size() && buffer[i] == 0)
    {
        i++;
    }

    std::string output = "";

    for (; i < num1.size() + num2.size(); i++)
    {
        output += buffer[i] + '0';
    }

    delete[] buffer;
    return output;
}

int main()
{
	std::cout << Multiply("1234", "5678");
	return 0;
}