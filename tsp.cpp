#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
using namespace std;

const int radius = 6371; //radius of earth in km

class City {
	int id;
	float lat,lon;
	string name;

	public:
	City();
	City (int i, string n, float x, float y);
	int getID(void);
	string getName(void);
	float getLat(void);
	float getLon(void);
	float getLatR(void); //Lattitude in radians
	float getLonR(void); //Longtitude in radians
	float dist(City);
};

City::City() {
	id = -1;
	lat = 0;
	lon = 0;
	name = "Undefined";
}

City::City (int i, string n, float x, float y) {
	id = i;
	lat = x;
	lon = y;
	name = n;
}
int City::getID() {
	return id;
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
		int getNoCities(void);
};
Cities::Cities(const char* filename){
	string line, name;
	float lat,lon;
	ifstream myfile(filename);
	istringstream ss;
	int n;
	int i = 0, j = 0;

	if (myfile.is_open())
	{
		getline (myfile,line); //skip first line
		i = 0;
		while (getline (myfile,line))
		{
			ss.str(line);
			ss >> lat;
			ss >> lon;
			ss >> name;
			carray.push_back(City(i,name,lat,lon));
			i++;
		}
		myfile.close();
	}
	else cout << "Unable to open file\n";
	// Prepare array with city - city distances 
	n = carray.size();
	dist = new float *[n];
	for (i=0;i<n;i++) {
		dist[i] = new float[n];
	}
	for (i=0;i<n;i++) {
		for (j=0;j<i;j++) {
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
int Cities::getNoCities() {
	return carray.size();
}


class Tour {
	int *tour;
	float tourLength;
	const char* fname;
	void shuffle(int *a, int n);
	float calcTourLength(int *a, int n, Cities cities);
	//Monte-Carlo Simulated Annealing
	void MC_SA(int *A, int n, Cities cities); 
	public:
	Tour(const char* filename);
	void restart();
	float getTourLength(void);
	int *getTour(void);
};

Tour::Tour(const char* filename) {
	fname = filename;
	srand (time(NULL));
	Cities cities(filename);
	int n = cities.getNoCities();
	tour = new int[n];
	for (int i=0; i<n; i++) {
		tour[i] = i;
	}
	//Prepare random tour
	this -> shuffle(tour,n);
	//Find optimal tour
	this -> MC_SA(tour, n, cities);
	tourLength = this -> calcTourLength(tour, n, cities);
}

void Tour::shuffle(int *a, int n) {
	// Shuffle the array with n elements
	// Fisher-Yates shuffle
	int x, tmp;
	for (int i=n-1; i>0; i--) {
		x = rand() % i;
		tmp = a[x];
		a[x] = a[i];
		a[i] = tmp;
	}
}

float Tour::calcTourLength(int *a, int n, Cities cities) {
	float l = 0;

	for(int i=1;i<n;i++){
		l = l + cities.getDist(a[i],a[i-1]);
	}
	//last city with first city
	l = l + cities.getDist(a[n-1],a[0]);
	//cout << "tour length: " << l << "\n";
	return l;
}

void Tour::MC_SA(int *A, int n, Cities cities){
	float startTemp = 1e+10; // start Temperature
	float endTemp = 1;       // ending Temperature
	float coolF = 0.99;      // cooling factor
	float temp;
	float tourL;   // current tour length;
	float newTourL; // new tour length
	float delta;

	int i,j,tmpi,tmpj,x;

	tourL = this -> calcTourLength(A, n, cities);
	temp = startTemp;

	while (temp > endTemp) {
		cout << "Tour : " << tourL << "\n";
		i = rand() %n;
		j = rand() %n;
		tmpi = A[i];
		tmpj = A[j];
		A[i] = tmpj;
		A[j] = tmpi;
		newTourL = this -> calcTourLength(A, n, cities);
		delta = newTourL - tourL;
		if (delta < 0 || exp(-delta/temp) > (rand() % 101)/100.0) {
			tourL = newTourL;
		}
		else {
			//revert to old configuration
			A[i] = tmpi;
			A[j] = tmpj;
		}
		temp = temp * coolF;
	}
}	

float Tour::getTourLength() {
	return tourLength;
}

int *Tour::getTour() {
	return tour;
}

int main() 
{
	Tour tour("cityList.txt");
	float l = tour.getTourLength();
	int *t = tour.getTour();
	cout << "Final tour length: " << l << "\n";
	

//	Cities cities("cityList.txt");
//	City Warszawa = cities.getCity(36);
//	City Oslo = cities.getCity(35);
//	cout << Warszawa.getName() << " to " << Oslo.getName() <<"\n";
//	cout << Warszawa.dist(Oslo) <<"\n\n";
//	cout <<"Matrix: "<<cities.getDist(36,35) << "\n";
}
