#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
using namespace std;

const int radius = 6371; //radius of earth in km

class City {
	float lat,lon;
	char const *name;

	public:
	City ();
	City (char const *n, float x, float y);
	const char* getName(void);
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

City::City (char const *n, float x, float y) {
	lat = x;
	lon = y;
	name = n;
}
const char* City::getName() {
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
	vector <Cities> carray;
	public:
		Cities(const char* filename);
};

Cities::Cities(const char* filename){
	string line;
	ifstream myfile (filename);
	if (myfile.is_open())
	{
		while (getline (myfile,line,','))
		{
			cout << line << "\n";
		}
		myfile.close();
	}
	else cout << "Unable to open file\n";
}


int main() 
{
	Cities cities("cityList.csv");
	City Warszawa("Warszawa",52.2333,21.0167);
	City Wroclaw("Wroclaw",51.1,17.0333);

	cout << Wroclaw.getName() << " to " << Warszawa.getName() <<"\n";

	cout << Wroclaw.dist(Warszawa) <<"\n";
	cout << Warszawa.dist(Wroclaw) <<"\n";
}
