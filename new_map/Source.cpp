#include <iostream>
#include <string>
#include <map>
#include "new_map.h"

int main()
{
	new_map<int, std::string> mapa;
	std::string lol = "sosi";
	for (int i = 0; i < 20; i++)
		mapa.insert(i, "sss");
	
	new_map<int, std::string>::iterator iter = mapa.begin();
	new_map<int, std::string>::iterator it = mapa.end();
	int i = 0;
	for (; iter != it; iter++)
		std::cout << i++ << std::endl;

	std::cout << mapa.find(2)->first << std::endl;
	mapa.print(std::cout);
	getchar();
	return 0;
}