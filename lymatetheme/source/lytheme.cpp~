#include <string>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <time.h>
#include <unistd.h>
#include <ctime>
#include <math.h>
#include <stdio.h>

using namespace std;

/* Some constants */
#define J2000 946728000 /* J2000.0 in seconds since Unix epoch */    
#define RADS 0.0174532925199433
#define DEGS 57.2957795130823
#define PI 3.1415927

/* returns an angle in degrees in the range 0 to 360 */
double range(double x) {
    double a, b;
    b = x / 360;
    a = 360 * (b - floor(b));
    if (a < 0)
    	a = 360 + a;
    return(a);
}

void sun_position(double LONG, double LAT, double &alt, double &az) 
{
    time_t now;
    double d, t, L, g, lambda;
    double epsilon, y, x, alpha, delta;
    double lst, ha;

    /*  Get Unix time in seconds UT then find
    days and Julian centuries since J2000.0 */

    now = time(NULL);
    if (now == -1) {
        puts("The time() function failed");
    	}
    
    d = (now - J2000)/(86400.0); /* days since J2000.0 */
    /* d = -877.04167; test from Meeus */
    L = range(280.461 + 0.9856474 * d);
    g = range(357.528 + 0.9856003 * d);
    lambda = range(L + 1.915 * sin(g*RADS) + 0.020 * sin(2*g*RADS));
    epsilon = 23.439 - 0.0000004 * d;
    y = cos(epsilon*RADS) * sin(lambda*RADS);
    x = cos(lambda*RADS);
    alpha = atan2(y , x);
    delta = asin(sin(epsilon*RADS)*sin(lambda*RADS));
    lst = range(280.46061837 + 360.98564736629 * d + LONG);
    //printf(" %3.0f ", lst);
    ha = range(lst - alpha*DEGS);
    alt = asin(sin(delta) * sin(LAT*RADS) + cos(delta) * cos(LAT*RADS) * cos(ha*RADS));
    //printf(" %3.0f ", alt*DEGS);
    y = -cos(delta) * cos(LAT*RADS) * sin(ha*RADS);
    x = sin(delta) - sin(LAT*RADS) * sin(alt);
    az = range(atan2(y, x)*DEGS);
    //printf(" %3.0f\n", az);
    alt = alt * DEGS;
}

int main(int argc, char *argv[])
{
	string lon = argv[1];
	string lat = argv[2];
	string light_theme = argv[3];
	string dark_theme = argv[4];

	double alt;
	double azi;

	sun_position((double)stof(lon), (double)stof(lat), alt, azi);

	if(alt < 0)
		cout << dark_theme << endl;
	else
		cout << light_theme << endl;
		
				
}
