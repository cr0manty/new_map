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
	//std::cout<< 
	mapa.find(2).second = lol;
	std::cout << iter.first;
	//iter++;
	std::cout << iter.second;

	mapa.print();
	getchar();
	return 0;
}