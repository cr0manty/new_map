#include <iostream>
#include <string>
#include <map>
#include "new_map.h"

int main()
{
	new_map<int, std::string> mapa;
	
	for (int i = 0; i < 20; i++)
		mapa.insert(i, "sss");
	
	new_map<int, std::string>::iterator iter = mapa.begin();
	std::cout<< mapa.find(2).second;
	//std::cout << iter.first;
	////iter++;
	//std::cout << iter.second;


	getchar();
	return 0;
}