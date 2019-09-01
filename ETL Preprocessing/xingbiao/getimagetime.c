#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "var.h"
#include "function.h"

//get shooting time 
void get_imagetime(char *str)
{
	int year,day;
	int month;
	int hour, min, sec;
	int a,b;
	char tp[11],tp1[50];
	
	sscanf(str,"%[^_]s_%s",tp,tp1);
	
	if(strncmp(tp,"diff",4))
		sscanf(str,"%*[^_]_%*[^_]_%d.%d_%*s",&a,&b);
	else
		sscanf(str,"%*[^_]_%*[^_]_%*[^_]_%d.%d_%*s",&a,&b);
	
	//Calculating the year of shooting time
	year = a/10000;
	a = a%10000;
	year = year + 100;
	
	//Calculating the month of shooting time
	month = a/100;
	a = a%100;
	
	//Calculating the day of shooting time
	day = a;
	
	//Calculating the hour of shooting time
	hour = b/10000;
	b = b%10000;
	
	//Calculating the minute of shooting time
	min = b/100;
	b = b%100;
	
	//Calculating the second of shooting time
	sec = b;
	
	sprintf(image_time,"%d-%d-%d %d:%d:%d", year+1900, month, day, hour, min, sec);
	
	return;
	
}
