#include <iostream>
#include <string>
#include "new_map.h"
#include <map>

int main()
{
	std::new_map<int,std::string> mapa;
	std::string si = "sis";
	for (int i = 0; i < 10; i++)
		mapa.insert(i, "sss");

	mapa[24] = "new";

	std::new_map<int, std::string>::iterator iter = mapa.begin();
	std::new_map<int, std::string>::iterator it = mapa.end();

	(*iter).second = "kik";
	for (; iter != it; ++iter)
		std::cout << (*iter).first << " ";

	iter = mapa.begin();
	std::cout << std::endl;
	for (; it != iter; --it)
		std::cout << (*it).first << " ";

	std::cout << mapa.find(24)->second << std::endl;

	mapa.erase(5);
	if (mapa.find(5))
		std::cout << "npos" << std::endl;

	getchar();
	return 0;
}