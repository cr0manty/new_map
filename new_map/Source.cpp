#include <iostream>
#include <string>
#include <map>
#include "new_map.h"

int main()
{
	new_map<int, std::string> mapa;
	std::string si = "sis";
	for (int i = 0; i < 5; i++)
		mapa.insert(i, "sss");
	
	new_map<int, std::string>::iterator iter = mapa.begin();
	
	new_map<int, std::string>::iterator it = mapa.end();

	mapa.print(std::cout);
	getchar();
	return 0;
}