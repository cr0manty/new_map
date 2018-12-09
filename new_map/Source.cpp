#include <iostream>
#include <string>
#include "new_map.h"
#include <map>

void test();
void my_test();

int main()
{
	//test();
	my_test();

	getchar();
	return 0;
}

void test() {
	std::map<int, int> map;
	for (int i = 0; i < 10; i++)
		map.emplace(i, 0);

	std::map<int, int>::iterator iter = map.begin();
	std::map<int, int>::iterator it = map.end();

	map.erase(0);
	for (; iter != it; ++iter)
		std::cout << (*iter).first << " ";
}

void my_test()
{
	std::map<int, int> maps;
	stf::new_map<int, std::string> map;
	for (int i = 0; i < 10; i++)
		map.insert(i, "sss");

	stf::new_map<int, std::string> mapa(map);

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


}
