#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

const int radius = 6371; //radius of earth in km

class City {
	float lat,lon;
	string name;

	public:
	City ();
	City (string n, float x, float y);
	string getName(void);
	float getLat(void);
	float getLon(void);
	float getLatR(void); //Lattitude in radians
	float getLonR(void); //Laongtitude in radians
	float dist(City);
};

City::City() {
	lat = 0;
	lon = 0;
	name = "Undefined";
}

City::City (string n, float x, float y) {
	lat = x;
	lon = y;
	name = n;
}
string City::getName() {
	return name;
}

float City::getLat() {
	return lat;
}

float City::getLon() {
	return lon;
}

float City::getLatR() { 
	return lat * 3.14/180;
}

float City::getLonR() {
	return lon * 3.14/180;
}
float City::dist(City otherCity) {
	// Calculate distance between cities using haversine formula
	float dLat = this->getLatR() - otherCity.getLatR();
	float dLon = this->getLonR() - otherCity.getLonR();
	float d = pow(sin(dLat/2),2)
	      + cos(otherCity.getLatR())*cos(this->getLatR())
	      * pow(sin(dLon/2),2);
	d = sqrt(d);
	d = 2 * radius * asin(d);
	return d;
}

class Cities {
	vector <City> carray;
	float **dist;
	public:
		Cities(const char* filename);
		City getCity(int n);
		float getDist(int i, int j);
};

Cities::Cities(const char* filename){
	string line, name;
	float lat,lon;
	ifstream myfile(filename);
	istringstream ss;
	int n;

	if (myfile.is_open())
	{
		getline (myfile,line); //skip first line
		while (getline (myfile,line))
		{
			ss.str(line);
			ss >> lat;
			ss >> lon;
			ss >> name;
			carray.push_back(City(name,lat,lon));
		}
		myfile.close();
	}
	else cout << "Unable to open file\n";
	// Prepare array with city -city  distances 
	n = carray.size();
	dist = new float *[n];
	for (int i=0;i<n;i++) {
		dist[i] = new float[n];
	}
	for (int i=0;i<n;i++) {
		for (int j=0;j<i;j++) {
			City city1 = carray[i];
			City city2 = carray[j];
			dist[i][j] = city1.dist(city2);
		}
	}
}

City Cities::getCity(int n) {
	if (n<=carray.size()) return carray[n];
	else return City(); //return empty city
			
}

float Cities::getDist(int i, int j) {
	if (i < j) {
		int tmp = i;
		i = j;
		j = tmp;
	}	       
	return dist[i][j];
}

int main() 
{
	Cities cities("cityList.txt");
	City Warszawa = cities.getCity(36);
	City Oslo = cities.getCity(35);
	cout << Warszawa.getName() << " to " << Oslo.getName() <<"\n";
	cout << Warszawa.dist(Oslo) <<"\n\n";
	cout <<"Matrix: "<<cities.getDist(36,35) << "\n";
}
