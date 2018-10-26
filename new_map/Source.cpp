#include <iostream>
#include <string>
#include <map>
#include "new_map.h"

int main()
{
	new_map<int, std::string> mapa;
	std::string si = "sis";
	for (int i = 0; i < 455; i++)
		mapa.insert(i, "sss");


	new_map<int, std::string>::iterator iter = mapa.begin();
	
	new_map<int, std::string>::iterator it = mapa.end();

	new_map<std::string, int> map;
	map.insert("sss", 2);
	map.insert("sss", 1);
	map.insert("sss", 3);

	map.print(std::cout);
	
	mapa.print(std::cout);
	getchar();
	return 0;
}