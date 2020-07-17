#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;
struct ChiPhi
{
	int SoHieu;
	int GioBay;
	float GiaVe;
};
void DocCityFile(vector<string> &city)
{
	ifstream f("cityFile.txt");
	string str;
	while (!f.eof())
	{
		getline(f, str);
		city.push_back(str);
	}
	f.close();
}

void DocFlightFile(vector<string> city, ChiPhi **&map)
{
	int nCity = city.size();
	map = new ChiPhi*[nCity];
	for (int i = 0; i < nCity; i++)
		map[i] = new ChiPhi[nCity];
	for (int i = 0; i < nCity; i++)
	{
		for (int j = 0; j < nCity; j++)
		{
			map[i][j].GiaVe = 0;
			map[i][j].GioBay = map[i][j].SoHieu= 0;
		}
	}
	ifstream f("flightFile.txt");
	string str, str1, str2;
	while (!f.eof())
	{
		getline(f, str);
		int i=0, j, dong, cot, n = str.length();
		while (str[i] != ',')
			i++;
		str1 = str.substr(0, i);
		for (dong = 0; dong < nCity; dong++)
		if (city[dong].compare(str1)==0)
			break;
		i = i + 2;
		j = i;
		while (str[j]<'0' || str[j]>'9')
			j++;
		str2 = str.substr(i, j-i-1);
		for (cot = 0; cot < nCity; cot++)
		if (city[cot].compare(str2)==0)
			break;
		i = j;
		while (str[i] != ' ')
			i++;
		map[dong][cot].SoHieu = stoi(str.substr(j, i - j));
		j = ++i;
		while (str[j] != ' ')
			j++;
		map[dong][cot].GiaVe = stof(str.substr(i, j-i));
		map[dong][cot].GioBay = stoi(str.substr(j + 1));
	}
	f.close();
}
int minDistance(int dist[], bool sptSet[],int nCity)
{
	int min = INT_MAX, min_index;
	for (int v = 0; v < nCity; v++)
	if (sptSet[v] == false && dist[v] <= min)
		min = dist[v], min_index = v;

	return min_index;
}
int minCost(float cost[], bool sptSet[], int nCity)
{
	float min = INT_MAX;
	int min_index;
	for (int v = 0; v < nCity; v++)
	if (sptSet[v] == false && cost[v] <= min)
		min = cost[v], min_index = v;
	return min_index;
}

void printPath(int parent[], int src, int des, vector<string> city, ChiPhi **map)
{
	if (parent[des] == -1)
		return;
	printPath(parent, src, parent[des], city, map);
	src = parent[des];
	cout << "Flight #" << map[src][des].SoHieu << " from " << city[src]
		<< " to " << city[des] << " cost: $" << map[src][des].GiaVe << " Duration: " << map[src][des].GioBay*1.0 / 60 << "hours\n";
}
void printSolution(int dist,float cost, int src, int des,
	int parent[], vector<string> city, ChiPhi **map)
{
	printPath(parent, src, des,city,map);
	cout << "Total Cost ............ $" << cost << endl;
	cout << "Total Duration......... " << dist*1.0/60 << " hours\n";
}
// lấy ý tưởng từ https://www.geeksforgeeks.org/printing-paths-dijkstras-shortest-path-algorithm/
void dijkstra(ChiPhi **map, int type, int src, int des, vector<string> city, int min,int k, int i)
{
	if (k == 0)
		return;
	cout << "Route " << i << ": \n";
	int nCity = city.size();
	int *dist = new int[nCity];
	float *cost = new float[nCity];
	bool *sptSet = new bool[nCity];
	int *parent = new int[nCity];
	if (type == 1)
	{
		for (int i = 0; i < nCity; i++)
		{
			parent[i] = -1;
			dist[i] = 0;
			cost[i] = INT_MAX;
			sptSet[i] = false;
		}
		cost[src] = 0;
		for (int count = 0; count < nCity - 1; count++)
		{

			int u = minCost(cost, sptSet, nCity);
			sptSet[u] = true;
			for (int v = 0; v < nCity; v++)
			if (!sptSet[v] && map[u][v].GiaVe > 0 &&
				cost[u] + map[u][v].GiaVe < cost[v] && cost[u] + map[u][v].GiaVe > min)
			{
				parent[v] = u;
				dist[v] = dist[u] + map[u][v].GioBay;
				cost[v] = cost[u] + map[u][v].GiaVe;
			}
		}
		printSolution(dist[des], cost[des], src, des, parent, city, map);
		dijkstra(map, type, src, des, city, (int)cost[des], k - 1, i + 1);
	}
	else
	{
		for (int i = 0; i < nCity; i++)
		{
			parent[i] = -1;
			dist[i] = INT_MAX;
			cost[i] = 0;
			sptSet[i] = false;
		}
		dist[src] = 0;
		for (int count = 0; count < nCity - 1; count++)
		{

			int u = minDistance(dist, sptSet, nCity);
			sptSet[u] = true;
			for (int v = 0; v < nCity; v++)
			if (!sptSet[v] && map[u][v].GioBay>0 &&
				dist[u] + map[u][v].GioBay < dist[v] && dist[u] + map[u][v].GioBay > min)
			{
				parent[v] = u;
				dist[v] = dist[u] + map[u][v].GioBay;
				cost[v] = cost[u] + map[u][v].GiaVe;
			}
		}
		printSolution(dist[des], cost[des], src, des, parent, city, map);
		dijkstra(map, type, src, des, city, dist[des], k - 1 , i+1);
	}
}
void DocRequestFile(vector<string> city, ChiPhi **&map)
{
	int nCity = city.size();
	ifstream f("requestFile.txt");
	string str, str1, str2;
	getline(f, str);
	int k = stoi(str);
	while (!f.eof())
	{
		getline(f, str);
		int i = 0, j, dau, cuoi, n = str.length();
		while (str[i] != ',')
			i++;
		str1 = str.substr(0, i);
		i = i + 2;
		j = i;
		while (str[j]<'0' || str[j]>'9')
			j++;
		str2 = str.substr(i, j - i - 1);
		for (dau = 0; dau < nCity; dau++)
		if (city[dau].compare(str1) == 0)
			break;
		for (cuoi = 0; cuoi < nCity; cuoi++)
		if (city[cuoi].compare(str2) == 0)
			break;
		cout << "Request is to fly from " << str1 << " to " << str2 << ".\n";
		if (dau == nCity)
		{
			cout << "Route 1: \n";
			cout << "Sorry. HPAir does not serve " << str1 << ".\n";
		}
		else if (cuoi == nCity)
		{
			cout << "Route 1: \n";
			cout << "Sorry. HPAir does not serve " << str2 << ".\n";
		}
		else
			dijkstra(map, stoi(str.substr(j)), dau, cuoi, city, 0, k, 1), cout << "\n";
	}
	f.close();
}