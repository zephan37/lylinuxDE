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

struct Wallpaper_config
{
	string wallpaper_type;
	float azi[100];
	float alt[100];
	float time[100];
	int count;
};

string get_substr(string source, string f, string r)
{
	int pos_1 = source.find(f) + f.length(); 
	int pos_2 = source.find(r, pos_1);
	return source.substr(pos_1, pos_2 - pos_1);
}

int get_substr_count(string source, string substr)
{
	int count = 0;
	int pos = source.find(substr);
	while(pos != string::npos) {
		count++;
		pos = source.find(substr, pos + 1);
	}

	return count;
}

void cfg_read(string config_path, Wallpaper_config &cfg)
{
	cfg.wallpaper_type = "";
	memset(cfg.azi, 0, sizeof cfg.azi);
	memset(cfg.alt, 0, sizeof cfg.alt);
	memset(cfg.time, 0, sizeof cfg.time);
	cfg.count = 0;

	ifstream infile(config_path.append("/config.li"));
	stringstream buffer;
	buffer << infile.rdbuf();
	string cfg_stream(buffer.str());

	//wallpaper_type	
	cfg.wallpaper_type = get_substr(cfg_stream, "[wallpaper_type {", "}]");
	
	//azi
	string azi_tmp = get_substr(cfg_stream, "[azi", "]");
	int azi_count = get_substr_count(azi_tmp, "{");

	if(azi_count == 0) {
		memset(cfg.azi, 0, sizeof cfg.azi);
	}
	else {
		string tmp;
		cfg.count = azi_count;
		for(int i = 0; i < azi_count; i++) {
			tmp = get_substr(azi_tmp, "{", "}");
			cfg.azi[i] = stof(tmp);	
			azi_tmp = azi_tmp.substr(azi_tmp.find(tmp) + tmp.length());
		}
	}

	//alt
	string alt_tmp = get_substr(cfg_stream, "[alt", "]");
	int alt_count = get_substr_count(alt_tmp, "{");

	if(alt_count == 0) {
		memset(cfg.alt, 0, sizeof cfg.alt);
	}
	else {
		string tmp;
		cfg.count = alt_count;
		for(int i = 0; i < alt_count; i++) {
			tmp = get_substr(alt_tmp, "{", "}");
			cfg.alt[i] = stof(tmp);	
			alt_tmp = alt_tmp.substr(alt_tmp.find(tmp) + tmp.length());
		}
	}

	//time
	string time_tmp = get_substr(cfg_stream, "[time", "]");
	int time_count = get_substr_count(time_tmp, "{");

	if(time_count == 0) {
		memset(cfg.time, 0, sizeof cfg.time);
	}
	else {
		string tmp;
		cfg.count = time_count;
		for(int i = 0; i < time_count; i++) {
			tmp = get_substr(time_tmp, "{", "}");
			cfg.time[i] = stof(tmp);	
			time_tmp = time_tmp.substr(time_tmp.find(tmp) + tmp.length());
		}
	}

	infile.close();

}

string get_exec_dir()
{
	char  current_absolute_path[256];
	//获取当前程序绝对路径
	int  cnt = readlink("/proc/self/exe", current_absolute_path, 256);
	if  (cnt < 0 || cnt >= 256)
	{
    		printf("***Error***\n");
		return "";
	}
	
	//获取当前目录绝对路径，即去掉程序名
	int  i;
	for(i = cnt; i >=0; --i)
	{
    		if  (current_absolute_path[i] == '/')
    		{
        		current_absolute_path[i+1] = '\0';
        		break;
    		}
	}

	return current_absolute_path;
}

string get_random_wallpaper()
{
	int count = 0;
	string path = get_exec_dir();
	path.append("wallpaper");
	for(const auto & entry : filesystem::directory_iterator(path)) {
		count++;
	}

	int i = 0;
	int random_n;
	srand((int)time(NULL));
	random_n = rand() % count;
	for(const auto & entry : filesystem::directory_iterator(path)) {
		i++;
		if(random_n == i - 1)
			return entry.path();
	}

	return "";
}

/*
void sun_position(double Longitude, double Latitude, int year, int month, int day, int hour, int minute, int second, double& height, double& direction)
{
	//赤纬计算	
	double N  = floor(month*275.0/9.0)-floor((month+9)/12.0)*(floor((year-4*floor(year/4.0)+2)/3.0)+1)+day-30;	//年积日计算
											
	double theta;
	double B = 2 * 3.1415926 * (N - 1) / 365; //弧度
	theta = 0.006918-0.399912*cos(B)+0.070257*sin(B)-0.006758*cos(2*B)+0.000907*sin(2*B)-0.002697*cos(3*B)+0.00148*sin(3*B);//弧度
	
	//计算太阳时差
	double B_0 = 2 * 3.1415926 * N / 365; //年积日对应角度
	double B_1 = B_0 * 3.1415926 / 180; //转换为弧度
	double EOT = 0.0172+0.4281*cos(B_1)-7.3515*sin(B_1)-3.3495*cos(2*B_1)-9.3619*sin(2*B_1);   //近似真太阳时差（以分为单位）

	//计算经度时差
	double A = 4.0 * (120 - Longitude); 

	//真太阳时
	double ST = (hour + minute/60 + second/3600) - A/60 + EOT/60;

	//高度角
	height = asin(sin(Latitude * 3.1415926 / 180) * sin(theta) + cos(Latitude * 3.1415926 / 180) * cos(theta) * cos(15*(ST - 12)*3.1415926/180));

	//方位角
	direction = acos((sin(height) * sin(Latitude*3.1415926/180) - sin(theta)) / (cos(height) * cos(Latitude * 3.1415926/180))); 
	direction = direction * 180 / 3.1415926;
	if(hour - (120 - Longitude)/15 > 12)
		direction = direction + 180;
	else
		direction = 180 - direction;

	height = height * 180 / 3.1415926;
}*/

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

int get_pic_sun(Wallpaper_config pic_cfg, float lon, float lat)
{
	double azi, alt;
	double min;
	int min_pos;

	sun_position((double)lon, (double)lat, alt, azi);

	min = pow(azi - pic_cfg.azi[0], 2) + pow(alt - pic_cfg.alt[0], 2);
	min_pos = 0;
	for(int i = 0; i < pic_cfg.count; i++) {
		if((pow(azi - pic_cfg.azi[i], 2) + pow(alt - pic_cfg.alt[i], 2)) < min) {
			min = pow(azi - pic_cfg.azi[i], 2) + pow(alt - pic_cfg.alt[i], 2);
			min_pos = i;
		}
	}

	return min_pos;
}

int get_pic_time(Wallpaper_config pic_cfg)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);
	
	double min;
	int min_pos;

	float time_sec = (ltm->tm_hour*60*60 + ltm->tm_min * 60 + ltm->tm_sec)/(24.0*60*60);

	min = abs(time_sec - pic_cfg.time[0]);
	min_pos = 0;
	for(int i = 0; i < pic_cfg.count; i++) {
		if(abs(time_sec - pic_cfg.time[i]) < min) {
			min = abs(time_sec - pic_cfg.time[i]);
			min_pos = i;
		}
	}

	return min_pos;
}

int main(int argc, char *argv[])
{
	if(argc != 4) {
		perror("please input a right parameter, eg: main [longitude] [Latitude] [mode]\n mode: random --- get random wallpaper\n       xxx --- name of wallpaper\n");
		return 0;
	}

	string lon = argv[1];
	string lat = argv[2];
	string mode = argv[3];

	string wallpaper_dir;
	if(0 == mode.compare("random")) {
		wallpaper_dir = get_random_wallpaper();
	}
	else {
		wallpaper_dir = get_exec_dir();
		wallpaper_dir.append("wallpaper/");
		wallpaper_dir.append(mode);
	}
		
	Wallpaper_config wallpaper_cfg;
	cfg_read(wallpaper_dir, wallpaper_cfg);

	if(0 == wallpaper_cfg.wallpaper_type.compare("sun")) {
		cout << wallpaper_dir << "/";
		cout << get_pic_sun(wallpaper_cfg, stof(lon), stof(lat)) << endl;
	}
	else if(0 == wallpaper_cfg.wallpaper_type.compare("time")) {
		cout << wallpaper_dir << "/";
		cout << get_pic_time(wallpaper_cfg) << endl;
	}
	
}
