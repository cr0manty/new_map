#include <iostream>
#include <string>
#include "new_map.h"
#include <map>

int main()
{
	std::map<int, std::string> ki;

	std::new_map<int,std::string> mapa;
	std::string si = "sis";
	for (int i = 5; i < 10; i++)
		mapa.insert(i, "sss");


	std::new_map<int, std::string>::iterator iter = mapa.begin();
	std::new_map<int, std::string>::iterator it = mapa.end();

	(*iter).second = "kik";
	
	/*for (; iter != it; ++iter)
		std::cout << (*iter).first << " ";
*/
	std::cout << mapa[1].first << std::endl;
	std::cout << mapa.find(0)->second << std::endl;

	mapa.erase(5);
	if (mapa.find(1) != mapa.npos)
		std::cout << "npos" << std::endl;

	std::cout << mapa.find(1)->second << std::endl;

	getchar();
	return 0;
}