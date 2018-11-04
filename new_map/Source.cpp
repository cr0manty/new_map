#include <iostream>
#include <string>
#include "new_map.h"
#include <map>

int main()
{
	std::map<int, std::string> ki;

	new_map<int, std::string> mapa;
	std::string si = "sis";
	for (int i = 0; i < 4; i++)
		mapa.insert(i, "sss");

	new_map<int, std::string>::iterator iter = mapa.begin();
	new_map<int, std::string>::iterator it = mapa.end();
	
	//iter.second = si;
	for (; iter != it; iter++)
		std::cout << iter.first << " ";
	iter.second = "sds";
	//mapa.emplace(1, "sdsd");
	std::cout << std::endl;
	std::cout << mapa.find(0)->second << std::endl;

	mapa.print(std::cout);
	getchar();
	return 0;
}