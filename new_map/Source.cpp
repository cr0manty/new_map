#include <iostream>
#include <string>
#include "new_map.h"

int main()
{
	new_map<int, std::string> mapa;
	std::string si = "sis";
	for (int i = 0; i < 4; i++)
		mapa.insert(i, "sss");

	new_map<int, std::string>::iterator iter = mapa.begin();
	new_map<int, std::string>::iterator it = mapa.end();
	
	for (; iter != it; iter++)
		std::cout << iter.first << " ";

	//std::cout << mapa.find(1)->second << std::endl;

	//mapa.print(std::cout);
	getchar();
	return 0;
}