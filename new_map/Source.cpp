#include <iostream>
#include <string>
#include "new_map.h"

int main()
{
	stf::new_map<int,std::string> mapa;
	for (int i = 0; i < 10; i++)
		mapa.insert(i, "sss");

	mapa[24] = "new";

	stf::new_map<int, std::string>::iterator iter = mapa.begin();
	stf::new_map<int, std::string>::iterator it = mapa.end();
	(*iter).second = "kik";
	for (; iter != it; ++iter)
		std::cout << (*iter).first << " ";

	iter = mapa.begin();
	std::cout << std::endl;
	for (; it != iter; --it)
		std::cout << (*it).first << " ";

	mapa.erase(5);
	if (!mapa.find(5))
		std::cout << "npos ";

	std::cout << mapa.find(24)->second << std::endl;
	
	for (auto i : mapa)
		std::cout << i.first << " ";


	getchar();
	return 0;
}