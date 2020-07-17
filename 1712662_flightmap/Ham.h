#include<iostream>
#include<fstream>
#include<string>
#include<vector>
struct ChiPhi
{
	int SoHieu;
	int GioBay;
	float GiaVe;
};
using namespace std;
void DocCityFile(vector<string> &city);
void DocFlightFile(vector<string> city, ChiPhi **&map);
int minCost(float cost[], bool sptSet[], int nCity);
int minDistance(int dist[], bool sptSet[], int nCity);
void printPath(int parent[], int src, int des, vector<string> city, ChiPhi **map);
void printSolution(int dist, float cost, int src, int des,
	int parent[], vector<string> city, ChiPhi **map);
void dijkstra(ChiPhi **map, int type, int src, int des, vector<string> city, int i);
void DocRequestFile(vector<string> city, ChiPhi **&map);