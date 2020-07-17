#include"Ham.h"
void main()
{
	vector<string> city;
	DocCityFile(city);
	ChiPhi **map;
	DocFlightFile(city, map);
	DocRequestFile(city, map);
}