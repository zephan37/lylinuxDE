#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define ALS_DEVICE_PATH "/sys/bus/iio/devices/iio:device0/in_illuminance_raw"
#define BRIGHTNESS "/sys/class/backlight/intel_backlight/brightness"
#define MAX_BRIGHTNESS "/sys/class/backlight/intel_backlight/max_brightness"

int get_als_lm()
{
	FILE *fp;
	fp = fopen(ALS_DEVICE_PATH, "r");
	if(fp == NULL)
		perror("device open failed");
	
	char str[50];		
	if(NULL == fgets(str, 50, fp))
		perror("get als data failed");
	
	fclose(fp);
       	
	return atoi(str);
}

int get_now_backlight()
{
	FILE *fp_brightness;
	FILE *fp_max_brightness;
	char bri_str[50];
	char max_bri_str[50];	
	
	fp_brightness = fopen(BRIGHTNESS, "r");
	fp_max_brightness = fopen(MAX_BRIGHTNESS, "r");
	if(fp_brightness == NULL)
		perror("device open failed");
	if(fp_max_brightness == NULL)
		perror("device open failed");	

	if(NULL == fgets(bri_str, 50, fp_brightness))
		perror("get brightness data failed");
	
	if(NULL == fgets(max_bri_str, 50, fp_max_brightness))
		perror("get max_brightness data failed");

	
	fclose(fp_brightness);
	fclose(fp_max_brightness);

       	return 100*atoi(bri_str)/atoi(max_bri_str);
}

void set_brightness(int brightness)
{
	FILE *fp_brightness;
	FILE *fp_max_brightness;
	char max_bri_str[50];	
	
	fp_brightness = fopen(BRIGHTNESS, "w");
	fp_max_brightness = fopen(MAX_BRIGHTNESS, "r");
	
	if(fp_brightness == NULL)
		perror("device open failed");
	if(fp_max_brightness == NULL)
		perror("device open failed");	
	
	if(NULL == fgets(max_bri_str, 50, fp_max_brightness))
		perror("get max_brightness data failed");
	
	fprintf(fp_brightness, "%d", atoi(max_bri_str)*brightness/100);

	fclose(fp_brightness);
	fclose(fp_max_brightness);
}

int get_hour()
{
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	
	return t->tm_hour;
}

int main()
{
	int lm;
	int brightness;
	int new_brightness;
	int now_hour;
 
	while(1) {
		lm = get_als_lm();	
		brightness = get_now_backlight();
		now_hour = get_hour();		

		if(now_hour > 6 && now_hour < 18)
			new_brightness = 9*lm/20 + 4;
		else
			new_brightness = 22*lm/157 + 4;

		if(new_brightness >= 100)
			new_brightness = 100;

		if(abs(brightness - new_brightness) > 5) {
			if(brightness > new_brightness) {
				for(int i = brightness; i >= new_brightness; i--) {
					set_brightness(i);
					usleep(1000000/abs(brightness - new_brightness));		
				}
			}
			if(brightness < new_brightness) {
				for(int i = brightness; i <= new_brightness; i++) {
					set_brightness(i);
					usleep(1000000/abs(brightness - new_brightness));		
				}
			}
		}

		usleep(2000000);
	}
}

