#include <iostream>
#include <string>
#include <map>
#include "new_map.h"

int main()
{
	std::map<int, double> maps;
	//
	maps.emplace(1, 2);
	std::map<int, double>::iterator it= maps.begin();
	std::cout << (*it).first;
	//std::cout << (*it).first << std::endl;
	for (int i = 0; i < 20; i++)
		maps.emplace(i, 213);
	maps.clear();

	new_map<int, std::string> mapa;
	new_map<int, std::string>::iterator iter;
	
	for (int i = 0; i < 20; i++)
		mapa.insert(i, "sss");
	iter = mapa.begin();
	std::cout << iter.second;
	//mapa.print();

	getchar();
	return 0;
}