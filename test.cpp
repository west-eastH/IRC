#include <iostream>

int main()
{
	std::string a;
	while (1)
	{
		std::cin >> a;
		for (size_t i = 0; i < a.length(); i++)
			if (!std::isprint(a[i]))
			{
				std::cout << "here" << std::endl;
			}
		
		std::cout << "length (" << a.length() << ")";
		std::cout << "string (" << a << ")";
	}
	return 0;
}